#pragma once

#include "zmq.hpp"
#include <string>
#include <iostream>

constexpr int PORT = 5555;

enum MSG_ID{
    LOSE,
    WIN,
    FAIL,
    SUCCESS,
    QUIT,
    ONLINE,
    CONNECT,
    STAT,
    WAIT,
    PLAY,
    GIVE_UP,
    SHOT,
    KILL,
    WOUND,
    MISS,
};

struct msg_t{
    MSG_ID id;
    std::string data;
    msg_t & operator =(const std::string & other){
        id = static_cast<MSG_ID>(other[0] - '0');
        data.clear();
        for(int i = 1; i < other.length(); i++){
            data.push_back(other[i]);
        }
        return *this;
    }
};
