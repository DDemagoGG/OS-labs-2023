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

double doExperiment(int K, int curRound, int firstPoints, int secondPoints){
    int curFirstPoints = firstPoints;
    int curSecondPoints = secondPoints;
    for (size_t i = 0; i <= K - curRound ; i++){
        curFirstPoints += rand() % 6 + rand() % 6 + 2;
        curSecondPoints += rand() % 6 + rand() % 6 + 2;
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
    pthread_mutex_t mutex;
    mutex_create(&mutex, nullptr);
    experiment* data = (experiment *) input;
    int K = data->base[0];
    int curRound = data->base[1];
    int firstPoints = data->base[2];
    int secondPoints = data->base[3];                   
    int testsNum = data->base[4];
    double res;
    for (int i = 0; i < testsNum; i++){
        res += doExperiment(K, curRound, firstPoints, secondPoints);
    }
    mutex_lock(&mutex);
    *data->win1 += res;
    mutex_unlock(&mutex);
    mutex_delete(&mutex);
    return 0;
}

double* game(int threadsNum, int K, int curRound, int firstPoints, int secondPoints, int testsNum){
    srand(time(nullptr));
    experiment argList;
    argList.base = new int[5];
    argList.base[0] = K;
    argList.base[1] = curRound;
    argList.base[2] = firstPoints;
    argList.base[3] = secondPoints;
    argList.base[4] = testsNum;
    double win1 = 0;
    argList.win1 = &win1;
    if (threadsNum > 1){
        int realThreadsNum = std::min(threadsNum, testsNum);
        pthread_t threads[realThreadsNum];
        int surplusTests = testsNum % realThreadsNum;
        for (int i = 0; i < realThreadsNum; i++){
            if (realThreadsNum == testsNum){
                argList.base[4] = 1;
            } else{
                if (surplusTests == 0){
                   argList. base[4] = testsNum / realThreadsNum;
                } else{
                    argList.base[4] = surplusTests + testsNum / realThreadsNum;
                    surplusTests = 0;
                }
            }
            thread_create(&threads[i], nullptr, doExperiments, &argList);
        }
        for(int i = 0; i < realThreadsNum; i++){
            pthread_join(threads[i], nullptr);
        }

    } else{
        doExperiments(&argList);
    }
    double * result = new double[2];
    result[0] = *argList.win1 / (double)testsNum;
    result[1] = (testsNum - *argList.win1) / (double)testsNum;
    delete[] argList.base;
    return result;
}