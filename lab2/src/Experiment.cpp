#include "Experiment.hpp"

void thread_create(pthread_t* thread, const pthread_attr_t* attr, void *(*start)(void *), void* arg) {
    if (pthread_create(thread, attr, start, arg) != 0) {
        perror("thread_create error!");
        exit(-1);
    }
}

void mutex_create(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr){
    if(pthread_mutex_init(mutex, attr) != 0){
        perror("mutex_create error!");
        exit(-1);
    }
}

void mutex_delete(pthread_mutex_t *mutex){
    if(pthread_mutex_destroy(mutex) != 0){
        perror("mutex_delete error!");
        exit(-1);
    }
}

void mutex_lock(pthread_mutex_t *mutex){
    if(pthread_mutex_lock(mutex) != 0){
        perror("mutex_lock error!");
        exit(-1);
    }
}

void mutex_unlock(pthread_mutex_t *mutex){
    if(pthread_mutex_unlock(mutex) != 0){
        perror("mutex_unlock error!");
        exit(-1);
    }
}

double doExperiment(int K, int curRound, int firstPoints, int secondPoints, unsigned int * seed){
    int curFirstPoints = firstPoints;
    int curSecondPoints = secondPoints;
    for (int i = 0; i <= K - curRound ; i++){
        curFirstPoints += rand_r(seed) % 6 + rand_r(seed) % 6 + 2;
        curSecondPoints += rand_r(seed) % 6 + rand_r(seed) % 6 + 2;
    }
    if (curFirstPoints > curSecondPoints){
        return 1;
    } else if (curFirstPoints == curSecondPoints){
        return 0.5;
    } else{
        return 0;
    }
}

void* doExperiments(void* input){
    experiment* data = (experiment *) input;
    int K = data->base[0];
    int curRound = data->base[1];
    int firstPoints = data->base[2];
    int secondPoints = data->base[3];                   
    int testsNum = data->testsNum;
    unsigned int seed = data->randSeed;
    double res = 0;
    for (int i = 0; i < testsNum; i++){
        res += doExperiment(K, curRound, firstPoints, secondPoints, &seed);
    }
    mutex_lock(data->mutex);
    *data->win1 += res;
    mutex_unlock(data->mutex);
    return 0;
}

double* game(int threadsNum, int K, int curRound, int firstPoints, int secondPoints, int testsNum){
    int * base = new int[4];
    base[0] = K;
    base[1] = curRound;
    base[2] = firstPoints;
    base[3] = secondPoints;
    double win1 = 0;
    pthread_mutex_t mutex;
    mutex_create(&mutex, nullptr);
    if (threadsNum > 1){
        int realThreadsNum = std::min(threadsNum, testsNum);      
        std::vector<experiment> argLists(realThreadsNum);
        std::vector<pthread_t> threads(realThreadsNum);
        int surplusTests = testsNum % realThreadsNum;
        for (int i = 0; i < realThreadsNum; i++){
            argLists[i] = {
                base, 0, &win1, (unsigned)time(nullptr) + i, &mutex
            };
            if (realThreadsNum == testsNum){
                argLists[i].testsNum = 1;
            } else{
                if (surplusTests == 0){
                   argLists[i]. testsNum = testsNum / realThreadsNum;
                } else{
                    argLists[i].testsNum = surplusTests + testsNum / realThreadsNum;
                    surplusTests = 0;
                }
            }
            thread_create(&threads[i], nullptr, doExperiments, &argLists[i]);
        }
        for(int i = 0; i < realThreadsNum; i++){
            pthread_join(threads[i], nullptr);
        }

    } else{
        experiment argList = {
            base, testsNum, &win1, (unsigned)time(nullptr), &mutex
        };
        doExperiments(&argList);
    }
    mutex_delete(&mutex);
    double * result = new double[2];
    result[0] = win1 / (double)testsNum;
    result[1] = (testsNum - win1) / (double)testsNum;
    delete[] base;
    return result;
}