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

void doExperiment(int K, int curRound, int firstPoints, int secondPoints, unsigned int * seed, int * res1, int * res2){
    int curFirstPoints = firstPoints;
    int curSecondPoints = secondPoints;
    for (int i = 0; i <= K - curRound ; i++){
        curFirstPoints += rand_r(seed) % 6 + rand_r(seed) % 6 + 2;
        curSecondPoints += rand_r(seed) % 6 + rand_r(seed) % 6 + 2;
    }
    if (curFirstPoints > curSecondPoints){
        *res1 += 1;
    } else if (curFirstPoints < curSecondPoints){
        *res2 += 1;
    } else{
        return;
    }
}

void* doExperiments(void* input){
    experiment* data = static_cast<experiment*> (input);
    int K = data->base[0];
    int curRound = data->base[1];
    int firstPoints = data->base[2];
    int secondPoints = data->base[3];                   
    int testsNum = data->testsNum;
    unsigned int seed = data->randSeed;
    int res1 = 0, res2 = 0;
    for (int i = 0; i < testsNum; i++){
        doExperiment(K, curRound, firstPoints, secondPoints, &seed, &res1, &res2);
    }
    mutex_lock(data->mutex);
    *data->win1 += res1;
    *data->win2 += res2;
    mutex_unlock(data->mutex);
    return 0;
}

std::array<double, 2> game(int threadsNum, int K, int curRound, int firstPoints, int secondPoints, int testsNum){
    std::array<int, 4> base;
    base[0] = K;
    base[1] = curRound;
    base[2] = firstPoints;
    base[3] = secondPoints;
    int win1 = 0;
    int win2 = 0;
    pthread_mutex_t mutex;
    mutex_create(&mutex, nullptr);
    if (threadsNum == 1){
        experiment argList = {
            base, testsNum, &win1, &win2, (unsigned)time(nullptr), &mutex
        };
        doExperiments(&argList);
    } else{
        int realThreadsNum = std::min(threadsNum, testsNum);      
        std::vector<experiment> argLists(realThreadsNum);
        std::vector<pthread_t> threads(realThreadsNum);
        for (int i = 0; i < realThreadsNum; i++){
            argLists[i] = {
                base, 0, &win1, &win2, (unsigned)time(nullptr) + i, &mutex
            };
            if (i == realThreadsNum - 1 && testsNum % realThreadsNum) {
                argLists[i].testsNum = testsNum - i * testsNum / realThreadsNum;
            } else {
                argLists[i].testsNum = testsNum / realThreadsNum;
            }
            thread_create(&threads[i], nullptr, doExperiments, &argLists[i]);
        }
        for(int i = 0; i < realThreadsNum; i++){
            pthread_join(threads[i], nullptr);
        }
    }
    mutex_delete(&mutex);
    std::array<double, 2> result;
    result[0] = win1 / (double)testsNum;
    result[1] = win2 / (double)testsNum;
    return result;
}