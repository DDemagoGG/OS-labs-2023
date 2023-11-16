#include <gtest/gtest.h>
#include <iostream>
#include <dlfcn.h>
#include <stdlib.h>

void * lib1_handle = dlopen(getenv("PATH_TO_LIB1"), RTLD_LAZY | RTLD_LOCAL);
void * lib2_handle = dlopen(getenv("PATH_TO_LIB2"), RTLD_LAZY | RTLD_LOCAL);
using ptr_PrimeCount = int (*)(int, int);
using ptr_Square = float (*)(float, float);
using ptr_CheckImplementation = int (*)();

TEST(PrimeCount, test_01){
    ptr_PrimeCount prime_count = (ptr_PrimeCount)dlsym(lib1_handle, "PrimeCount");
    int res = prime_count(1, 11);
    EXPECT_EQ(res, 5);
}

TEST(PrimeCount, test_02){
    ptr_PrimeCount prime_count = (ptr_PrimeCount)dlsym(lib2_handle, "PrimeCount");
    int res = prime_count(1, 11);
    EXPECT_EQ(res, 5);
}

TEST(Square, test_01){
    ptr_Square square = (ptr_Square)dlsym(lib1_handle, "Square");
    float res = square(2, 11);
    EXPECT_EQ(res, 22);
}

TEST(Square, test_02){
    ptr_Square square = (ptr_Square)dlsym(lib2_handle, "Square");
    float res = square(2, 11);
    EXPECT_EQ(res, 11);
    dlclose(lib1_handle);
    dlclose(lib2_handle);
}