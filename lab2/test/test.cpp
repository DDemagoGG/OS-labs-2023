#include <gtest/gtest.h>
#include "Experiment.hpp"

TEST(ThirdLabTests, SingleThreadYieldsCorrectResults){
    double* res;
    res = game(1, 1, 1, 0, 0, 1);
    EXPECT_TRUE((res[0] > 0.4 or res[0] < 0.6) and (res[1] > 0.4 or res[1] < 0.6));
    res = game(1, 10, 10, 15, 1, 10);
    EXPECT_TRUE((res[0] == 1) and (res[1] == 0));
}

TEST(ThirdLabTest, ThreadConfigurations){
    auto performTestForGivenSize = [](int K, int curRound, int firstPoints, int secondPoints, int testsNum, int maxThreadCount){
        double* res1, *res2;
        res1 = game(1, K, curRound, firstPoints, secondPoints, testsNum);
        for(int i = 2; i < maxThreadCount; ++i) {
            res2 = game(i, K, curRound, firstPoints, secondPoints, testsNum);
            EXPECT_TRUE((fabs(res1[0] - res2[0]) <= 0.1) and (fabs(res1[1] - res2[1]) <= 0.1));
            
        }
    };
    performTestForGivenSize(10, 1, 0, 0, 10000, 10);
    performTestForGivenSize(15, 2, 12, 2, 10000, 100);
    performTestForGivenSize(15, 14, 156, 26, 1, 50);
    performTestForGivenSize(10, 1, 0, 0, 10000, 50);
}

TEST(ThirdLabTest, PerfomanceTest) {
    auto getAvgTime = [](int threadNum) {
        double avg = 0;
        int runsCount = 10;
        for(int i = 0; i < runsCount; ++i) {
            auto begin = std::chrono::high_resolution_clock::now();
            game(threadNum, 10, 1, 0, 0, 500000);
            auto end = std::chrono::high_resolution_clock::now();
            avg += std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
        }
        return avg / runsCount;
    };

    auto singleThread = getAvgTime(1);
    auto multiThread = getAvgTime(2);

    std::cout << "Avg time for 1 thread: " << singleThread << '\n';
    std::cout << "Avg time for 2 threads: " << multiThread << '\n';

    EXPECT_GE(singleThread, multiThread);
}