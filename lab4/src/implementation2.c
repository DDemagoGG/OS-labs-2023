#include "lib.h"
#include <stdlib.h>

int CheckImplementation(){
    return 2;
}

int PrimeCount(int A, int B){
    int prime_counter = 0;
    int * segment = malloc(sizeof(int) * (B + 1));
    for(int i = 0; i <= B; i++){
        segment[i] = i;
    }
    segment[1] = 0;
    for(int i = 2; i <= B; i++){
        if (segment[i] != 0){
            if (i >= A){
                prime_counter++;
            }
            for(int j = i * i; j <= B; j += i){
                segment[j] = 0;
            }
        }
    }
    free(segment);
    return prime_counter;
}

float Square(float A, float B){
    return A * B / 2;
}