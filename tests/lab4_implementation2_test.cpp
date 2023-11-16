#include <gtest/gtest.h>
extern "C" {
    #include "lib.h"
}
#include <iostream>

TEST(PrimeCount, test_01){
    int res = PrimeCount(1, 11);
    EXPECT_EQ(res, 5);
}

TEST(Square, test_01){
    float res = Square(2, 10);
    EXPECT_EQ(res, 10);
}