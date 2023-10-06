#include <iostream>
#include <unistd.h>
#include <fstream>

int main(int argc, char* argv[]){
    if (argc < 2){
        perror("Too few arguments");
        exit(1);
    }
    std::ofstream res;
    res.open(argv[1]);
    int sum = 0;
    int buf;
    while(read(STDIN_FILENO, &buf, sizeof(int)) != 0){
        sum += buf;
    }
    res << sum;
    return 0;
}