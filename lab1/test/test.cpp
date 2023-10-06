#include <gtest/gtest.h>
#include <unistd.h>
#include <fstream>
#include "parent.hpp"

TEST(basic_test_set, test_01){
    std::fstream f;
    f.open("test");
    f << "test" << ' ' << 1 << ' ' << 2 << ' ' << 3;
    f.close(); 
    std::filebuf fb;
    fb.open("test", std::ios::in);
    std::istream is(&fb);
    processes(is);
    fb.close();
    int status;
    wait(&status);
    f.open("test");
    int res;
    f >> res;
    ASSERT_TRUE(res == 6);
}

TEST(basic_test_set, test_02){
    std::fstream f;
    f.open("test");
    f << "test" << ' ' << 0 << ' ' << 0 << ' ' << 0;
    f.close(); 
    std::filebuf fb;
    fb.open("test", std::ios::in);
    std::istream is(&fb);
    processes(is);
    fb.close();
    int status;
    wait(&status);
    f.open("test");
    int res;
    f >> res;
    ASSERT_TRUE(res == 0);
}

TEST(basic_test_set, test_03){
    std::fstream f;
    f.open("test");
    f << "test" << ' ' << -1 << ' ' << -2 << ' ' << -3;
    f.close(); 
    std::filebuf fb;
    fb.open("test", std::ios::in);
    std::istream is(&fb);
    processes(is);
    fb.close();
    int status;
    wait(&status);
    f.open("test");
    int res;
    f >> res;
    ASSERT_TRUE(res == -6);
}

TEST(basic_test_set, test_04){
    std::fstream f;
    f.open("test");
    f << "test" << ' ' << 11231 << ' ' << 342352 << ' ' << 344223;
    f.close(); 
    std::filebuf fb;
    fb.open("test", std::ios::in);
    std::istream is(&fb);
    processes(is);
    fb.close();
    int status;
    wait(&status);
    f.open("test");
    int res;
    f >> res;
    ASSERT_TRUE(res == 697806);
}

TEST(basic_test_set, test_05){
    std::fstream f;
    f.open("test");
    f << "test" << ' ' << -99 << ' ' << 9394 << ' ' << 0 << ' ' << 14234 << ' ' << 11111 << ' ' << 23233 << ' ' << 34523 << ' ' << 43523 << ' ' << 12412;
    f.close(); 
    std::filebuf fb;
    fb.open("test", std::ios::in);
    std::istream is(&fb);
    processes(is);
    fb.close();
    int status;
    wait(&status);
    f.open("test");
    int res;
    f >> res;
    ASSERT_TRUE(res == 148331);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}