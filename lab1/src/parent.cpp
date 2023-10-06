#include <iostream>
#include <unistd.h>
#include <fstream>
#include "parent.hpp"
#include <sys/types.h>
#include <sys/wait.h>

int fork_process(){
    pid_t pid = fork();
    if (pid == -1){
        perror("Fork error!\n");
        exit(-1);
    }
    return pid;
}

void create_pipe(int* pipe_fd){
    if (pipe(pipe_fd) == -1){
        perror("Pipe error!\n");
        exit(-1);
    }
}

void dup_fd(int oldfd, int newfd) {
    if (dup2(oldfd, newfd) == -1) {
        perror("dup2 error!\n");
        exit(-1);
    }
}

void processes(std::istream &input){
    const char* pathToChild = getenv("PATH_TO_CHILD");
    int pipe1_fd[2];
    create_pipe(pipe1_fd);
    std::string f;
    input >> f;
    pid_t child = fork_process();
    if (child == 0){
        close(pipe1_fd[1]);
        dup_fd(pipe1_fd[0], STDIN_FILENO);
        execl(pathToChild, pathToChild, f.c_str(), nullptr);
        close(pipe1_fd[0]);
    } else{
        close(pipe1_fd[0]);
        int number;
        while (input >> number){
            write(pipe1_fd[1], &number, sizeof(int));
        }
        close(pipe1_fd[1]);
        int status;
        wait(&status);
    }
}