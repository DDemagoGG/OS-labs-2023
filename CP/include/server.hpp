#pragma once

#include "common.hpp"
#include <sqlite3.h>
#include <map>

class Manager{
public:
    Manager();
    ~Manager();
    void Routine();
    MSG_ID CheckUser(std::string & login);
    void DeleteUser(std::string & login);
    std::string GetStat(std::string & login);
    MSG_ID SetStat(std::string & login, int game_res);
private:
    zmq::context_t context;
    zmq::socket_t socket;
    sqlite3 * db;
};