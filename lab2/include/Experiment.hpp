#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <array>
#include <chrono>

struct experiment{
    std::array<int, 4> base;
    int testsNum;
    int * win1;
    int * win2;
    unsigned int randSeed;
    pthread_mutex_t *mutex;
};

void thread_create(pthread_t* thread, const pthread_attr_t* attr, void *(*start)(void *), void* arg);
void mutex_create(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);
void mutex_delete(pthread_mutex_t *mutex);
void mutex_lock(pthread_mutex_t *mutex);
void mutex_unlock(pthread_mutex_t *mutex);
void doExperiment(int K, int curRound, int firstPoints, int secondPoints, unsigned int * seed, int * res1, int * res2);
void* doExperiments(void *);
std::array<double, 2> game(int threadsNum, int K, int curRound, int firstPoints, int secondPoints, int testsNum);