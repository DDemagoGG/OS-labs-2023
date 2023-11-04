#include <gtest/gtest.h>
#include <unistd.h>
#include <fstream>
#include "parent.hpp"

int check(std::string const input){
    std::istringstream buf("test " + input);
    std::istream is(buf.rdbuf());
    processes(is);
    std::fstream f;
    f.open("test");
    int res;
    f >> res;
    f.close();
    remove("./test");
    return res;
}

TEST(basic_test_set, test_01){
    EXPECT_EQ(check("1 2 3"), 6);
}

TEST(basic_test_set, test_02){
    EXPECT_EQ(check("0 0 0"), 0);
}

TEST(basic_test_set, test_03){
    EXPECT_EQ(check("-1 -2 -3"), -6);
}

TEST(basic_test_set, test_04){
    EXPECT_EQ(check("11231 342352 344233"), 697816);
}

TEST(basic_test_set, test_05){
    EXPECT_EQ(check("-99 9394 0 14234 11111 23233 34523 43523 12412"), 148331);
}

TEST(basic_test_set, test_06){
    EXPECT_DEATH(check("abc"), "Invalid input");
}

TEST(basic_test_set, test_07){
    EXPECT_DEATH(check(""), "Invalid input");
}

TEST(basic_test_set, test_08){
    EXPECT_DEATH(check("- -1"), "Invalid input");
}

TEST(basic_test_set, test_09){
    EXPECT_DEATH(check("2 3 4 iusadhfjish"), "Invalid input");
}