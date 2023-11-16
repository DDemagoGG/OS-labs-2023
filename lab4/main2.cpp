#include <dlfcn.h>
#include <stdlib.h>
#include <iostream>

void Interface(void * lib_handles[2], int start_mode){
    int mode = start_mode;
    using ptr_PrimeCount = int (*)(int, int);
    using ptr_Square = float (*)(float, float);
    using ptr_CheckImplementation = int (*)();
    ptr_PrimeCount prime_count = (ptr_PrimeCount)dlsym(lib_handles[mode], "PrimeCount");
    ptr_Square square = (ptr_Square)dlsym(lib_handles[mode], "Square");
    ptr_CheckImplementation check_implementation = (ptr_CheckImplementation)dlsym(lib_handles[mode], "CheckImplementation");
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
            std::cout << "h - help\nq - quit\n0 - switch implementation\n1 N M - launch PrimeCount function with arguments N and M\n2 N M - launch Square function with arguments N and M\n";
            break;
        case 'q':
            std::cout << "Bye!\n";
            program_end = 1;
            break;
        case '0':
            mode = 1 - mode;
            prime_count = (ptr_PrimeCount)dlsym(lib_handles[mode], "PrimeCount");
            square = (ptr_Square)dlsym(lib_handles[mode], "Square"); 
            check_implementation = (ptr_CheckImplementation)dlsym(lib_handles[mode], "CheckImplementation");
            std::cout << "Implementatiom switched! Current implementation - " << check_implementation() << ".\n";
            break;
        case '1':
            int n, m;
            std::cin >> n >> m;
            std::cout << prime_count(n, m) << '\n';
            break;
        case '2':
            int a, b;
            std::cin >> a >> b;
            std::cout << square(a, b) << '\n';
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
    void * lib_handles[2];
    lib_handles[0] = dlopen(getenv("PATH_TO_LIB1"), RTLD_LAZY | RTLD_LOCAL);
    lib_handles[1] = dlopen(getenv("PATH_TO_LIB2"), RTLD_LAZY | RTLD_LOCAL);
    Interface(lib_handles, 0);
    dlclose(lib_handles[0]);
    dlclose(lib_handles[1]);
}