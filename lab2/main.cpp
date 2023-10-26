#include "Experiment.hpp"

int main(int argc, char *argv[]){
    if (argc != 2){
        perror("Invalid input! Must be: THREAD_NUM ");
        exit(-1);
    }
    long threadsNum = strtol(argv[1], nullptr, 10);
    if (threadsNum < 0){
        perror("Invalid input! THREAD_NUM must be 0 or greater");
        exit(-1);
    } else if(threadsNum == 0){
        exit(-1);
    } else{
        int K, curRound, firsttPoints, secondPoints, testsNum;
        std::cin >> K >> curRound >> firsttPoints >> secondPoints >> testsNum;
        std::array<double, 2> result = game(threadsNum, K, curRound, firsttPoints, secondPoints, testsNum);
        std::cout << result[0] << " - вероятность победы 1 игрока\n" << result[1] << " - вероятность победы 2 игрока\n";
    }
    return 0;   
}