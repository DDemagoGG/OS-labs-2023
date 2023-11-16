#include "lib.h"

int CheckImplementation(){
    return 1;
}

int PrimeCount(int A, int B){
    int prime_counter = 0;
    for (int i = A; i <= B; i++){
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

float Square(float A, float B){
    return A * B;
}