#include <iostream>
#include <unistd.h>
#include <fstream>

int main(int argc, char* argv[]){
    std::ofstream res;
    res.open(argv[1]);
    int sum = 0;
    int buf;
    while(read(STDIN_FILENO, &buf, sizeof(int)) != 0){
        sum += buf;
    }
    res << sum;
    res.close();
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    return 0;
}