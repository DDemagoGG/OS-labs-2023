#include <iostream>
#include <unistd.h>
#include <fstream>

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
    int pipe1_fd[2], pipe2_fd[2];
    create_pipe(pipe1_fd);
    create_pipe(pipe2_fd);
    std::string f;
    input >> f;
    pid_t child = fork_process();
    if (child == 0){
        close(pipe1_fd[1]);
        close(pipe2_fd[0]);
        dup_fd(pipe2_fd[1], STDOUT_FILENO);
        dup_fd(pipe1_fd[0], STDIN_FILENO);
        execl("../build/child", "../build/child", f.c_str(), NULL);
        close(pipe1_fd[0]);
        close(pipe2_fd[1]);
    } else{
        int number;
        while (input >> number){
            write(pipe1_fd[1], &number, sizeof(int));
        }
        close(pipe1_fd[0]);
        close(pipe1_fd[1]);
        close(pipe2_fd[0]);
        close(pipe2_fd[1]);
    }
}