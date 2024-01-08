#pragma once

#include "common.hpp"
#include <random>

enum field_type{
    EMPTY,
    BUSY1,
    BUSY2,
    BUSY3,
    BUSY4,
    CANNOT_PLACE,
    BUSY,
    POINT,
    SUNK
};

class Gamer{
public:
    Gamer(std::string & port, int initiative);
    int Play(int initiative, std::string login);
    void CreateMap();
    void PrintMap();
    void PrintTwoMaps(std::string & enemy_login, int enemy_map[10][10]);
    int Attack(int enemy_map[10][10], int * sunk_counter);
    int Defend(int * sunk_counter);
private:
    int map[10][10];
    int move_number;
    zmq::context_t context;
    zmq::socket_t socket;
    int ConvertAlphToInt(char ch);
    int InitFieldAround(int str, int col);
    int KillTest(int str, int col);
};