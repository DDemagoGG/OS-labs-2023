#include "lib.h"

int CheckImplementation(){
    return 1;
}

int PrimeCount(int a, int b){
    int prime_counter = 0;
    for (int i = a; i <= b; i++){
        if (i == 1){
            continue;
        }
        ++prime_counter;
        for(int j = 2; j < i; j++){
            if(i % j == 0){
                --prime_counter;
                break;
            }
        }
    }
    return prime_counter;
}

float Square(float a, float b){
    return a * b;
}