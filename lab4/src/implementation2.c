#include "lib.h"
#include <stdlib.h>

int CheckImplementation(){
    return 2;
}

int PrimeCount(int a, int b){
    int prime_counter = 0;
    int * segment = malloc(sizeof(int) * (b + 1));
    for(int i = 0; i <= b; i++){
        segment[i] = i;
    }
    segment[1] = 0;
    for(int i = 2; i <= b; i++){
        if (segment[i] != 0){
            if (i >= a){
                prime_counter++;
            }
            for(int j = i * i; j <= b; j += i){
                segment[j] = 0;
            }
        }
    }
    free(segment);
    return prime_counter;
}

float Square(float a, float b){
    return a * b / 2;
}