#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <math.h>

struct experiment{
    int * base;
    double * win1;
};

void thread_create(pthread_t* thread, const pthread_attr_t* attr, void *(*start)(void *), void* arg);
void mutex_create(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);
void mutex_delete(pthread_mutex_t *mutex);
void mutex_lock(pthread_mutex_t *mutex);
void mutex_unlock(pthread_mutex_t *mutex);
double doExperiment(int K, int curRound, int firstPoints, int secondPoints);
void* doExperiments(void *);
double* game(int threadsNum, int K, int curRound, int firstPoints, int secondPoints, int testsNum);