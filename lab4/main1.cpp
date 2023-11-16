extern "C" {
    #include "lib.h"
}
#include <iostream>

void Interface(){
    char state;
    std::cout << "Hello, user! Enter <h> for help." << '\n';
    std::cout << "----------------------------------------------------------------" << '\n';
    int program_end = 0;
    while(!program_end){
        std::cin >> state;
        std::cout << "----------------------------------------------------------------" << '\n';
        switch (state)
        {
        case 'h':
            std::cout << "h - help\nq - quit\n1 N M - launch PrimeCount function with arguments N and M\n2 N M - launch Square function with arguments N and M\n";
            break;
        case 'q':
            std::cout << "Bye!\n";
            program_end = 1;
            break;
        case '1':
            int n, m;
            std::cin >> n >> m;
            std::cout << PrimeCount(n, m) << '\n';
            break;
        case '2':
            int a, b;
            std::cin >> a >> b;
            std::cout << Square(a, b) << '\n';
            break;
        default:
            std::cout << "Invalid input! Enter <h> for help." << '\n';
            std::cout << state << '\n';
            break;
        }
        std::cout << "----------------------------------------------------------------" << '\n';
    }
}

int main(){
    Interface();
    return 0;
}