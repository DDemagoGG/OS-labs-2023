#include <iostream>
#include <unistd.h>
#include <fstream>

void processes(std::istream &input);

int main(){
    processes(std::cin);
    return 0;
}