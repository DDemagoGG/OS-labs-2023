#pragma once

#include <iostream>
#include <unistd.h>
#include <fstream>
#include <sys/types.h>
#include <sys/wait.h>
#include <sstream>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h> 
#include <pthread.h>
#include <semaphore.h>
struct MMF{
    int * addr;
    int capacity;
    int fd;
};
int fork_process();
void * create_MMF(void *addr, size_t len, int prot, int flags, int fd, off_t offset);
int create_shm(const char * name, int oflag, mode_t mode);
sem_t * open_semaphor(const char *name, int oflag);
void unlock_semaphor(sem_t * sem);
void lock_semaphor(sem_t * sem);
void close_semaphor(sem_t * sem);
void remove_semaphor(const char *name);
void file_truncate(int fd, off_t length);
void remove_shm(const char * name);
std::string isValidInput(std::istream &input);
void extend_capacity_MMF(MMF * mem, int new_size);
void push_MMF(MMF * mem, int num);
void processes(std::istream &input);