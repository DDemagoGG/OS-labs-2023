#include <iostream>
#include <unistd.h>
#include <fstream>
#include "parent.hpp"
#include <sys/types.h>
#include <sys/wait.h>
#include <sstream>

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



 std::string isValidInput(std::istream &input){
    std::string s;
    getline(input, s);
    int thereIsNumbers = 0;
    int minusCounter = 0;
    for (char c : s){
        if (c != '-' and !isdigit(c) and c != ' '){
            perror("Invalid input!");
            exit(-1);
        } else if (c == '-'){
            minusCounter++;
        } else if (c == ' '){
            if (minusCounter){
                perror("Invalid input!");
                exit(-1);
            }
        } else{
            minusCounter = 0;
            thereIsNumbers = 1;
        }
    }
    if (!thereIsNumbers){
        perror("Invalid input!");
        exit(-1);
    }
    return s;
}

void processes(std::istream &input){
    const char* pathToChild = getenv("PATH_TO_CHILD");
    int pipe1_fd[2];
    create_pipe(pipe1_fd);
    std::string f;
    input >> f;
    std::istringstream buf(isValidInput(input));
    std::istream validInput(buf.rdbuf());
    pid_t child = fork_process();
    if (child == 0){
        close(pipe1_fd[1]);
        dup_fd(pipe1_fd[0], STDIN_FILENO);
        execl(pathToChild, pathToChild, f.c_str(), nullptr);
        close(pipe1_fd[0]);
    } else{
        close(pipe1_fd[0]);
        int number;
        while (validInput >> number){
            write(pipe1_fd[1], &number, sizeof(int));
        }
        close(pipe1_fd[1]);
        int status;
        wait(&status);
    }
}