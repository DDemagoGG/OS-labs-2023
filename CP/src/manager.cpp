#include "server.hpp"

Manager::Manager() : context{1}, socket{context, zmq::socket_type::rep}{
    socket.bind(("tcp://*:" + std::to_string(PORT)).c_str());
    char* path = getenv("PATH_TO_DATABASE");
    sqlite3_open(path, &db);
    char * sql = "CREATE TABLE IF NOT EXISTS users(login TEXT, online INTEGER, wins INTEGER, loses INTEGER)";
    sqlite3_exec(db, sql, 0, 0, nullptr);
}

Manager::~Manager(){
    sqlite3_close(db);
}

MSG_ID Manager::CheckUser(std::string & login){
    char * sql = "SELECT online FROM users WHERE login = ?";
    sqlite3_stmt *data1;
    sqlite3_prepare_v2(db, sql, -1, &data1, 0);
    const char * c_login = login.c_str();
    sqlite3_bind_text(data1, 1, c_login, -1, SQLITE_STATIC);
    int rc = sqlite3_step(data1);
    MSG_ID res = FAIL;
    if (rc == SQLITE_ROW){
        if (sqlite3_column_int(data1, 0) == 1){
            sqlite3_finalize(data1);
            res = ONLINE;
        } else{
            sqlite3_finalize(data1);
            sql = "UPDATE users SET online = 1 WHERE login = ?";
            sqlite3_stmt *data2;
            sqlite3_prepare_v2(db, sql, -1, &data2, 0);
            sqlite3_bind_text(data2, 1, c_login, -1, SQLITE_STATIC);
            sqlite3_step(data2);
            sqlite3_finalize(data2);
            res = SUCCESS;
        }
    } else{
        sqlite3_finalize(data1);
        sql = "INSERT INTO users (login, online, wins, loses) VALUES (?, 1, 0, 0)";
        sqlite3_stmt *data2;
        sqlite3_prepare_v2(db, sql, -1, &data2, 0);
        sqlite3_bind_text(data2, 1, c_login, -1, SQLITE_STATIC);
        sqlite3_step(data2);
        sqlite3_finalize(data2);
        res = SUCCESS;
    }
    return res;
}

void Manager::DeleteUser(std::string & login){
    char * sql = "UPDATE users SET online = 0 WHERE login = ?";
    sqlite3_stmt *data;
    if(sqlite3_prepare_v2(db, sql, -1, &data, 0) != SQLITE_OK){
        fprintf(stderr, "Error: %s\n", sqlite3_errmsg(db));
    }
    sqlite3_bind_text(data, 1, login.c_str(), -1, SQLITE_STATIC);
    sqlite3_step(data);
    sqlite3_finalize(data);
}

std::string Manager::GetStat(std::string & login){
    char * sql = "SELECT wins, loses FROM users WHERE login = ?";
    sqlite3_stmt *data;
    sqlite3_prepare_v2(db, sql, -1, &data, 0);
    sqlite3_bind_text(data, 1, login.c_str(), -1, SQLITE_STATIC);
    std::string result;
    while (sqlite3_step(data) == SQLITE_ROW){
        std::string wins = std::to_string(sqlite3_column_int(data, 0)); 
        std::string loses = std::to_string(sqlite3_column_int(data, 1));
        result = wins + '|' + loses;
    }
    return result;
}

MSG_ID Manager::SetStat(std::string & login, int game_res){
    if (game_res){
        char * sql = "UPDATE users SET wins = wins + 1 WHERE login = ?";
        sqlite3_stmt *data;
        if(sqlite3_prepare_v2(db, sql, -1, &data, 0) != SQLITE_OK){
            fprintf(stderr, "Error: %s\n", sqlite3_errmsg(db));
        }
        sqlite3_bind_text(data, 1, login.c_str(), -1, SQLITE_STATIC);
        sqlite3_step(data);
        sqlite3_finalize(data);
    } else{
        char * sql = "UPDATE users SET loses = loses + 1 WHERE login = ?";
        sqlite3_stmt *data;
        if(sqlite3_prepare_v2(db, sql, -1, &data, 0) != SQLITE_OK){
            fprintf(stderr, "Error: %s\n", sqlite3_errmsg(db));
        }
        sqlite3_bind_text(data, 1, login.c_str(), -1, SQLITE_STATIC);
        sqlite3_step(data);
        sqlite3_finalize(data);
    }
    return SUCCESS;
}

void Manager::Routine(){
    zmq::message_t reply;
    msg_t msg;
    int available_port = PORT + 1;
    std::map<std::string, int> waiting_pull;
    while(1){
        socket.recv(reply, zmq::recv_flags::none);
        msg = reply.to_string();
        switch (msg.id){
            case CONNECT:
                {
                MSG_ID connection_result = CheckUser(msg.data);
                socket.send(zmq::buffer(std::to_string(connection_result)));
                break;
                }
            case QUIT:
                {
                MSG_ID delete_result = SUCCESS;
                DeleteUser(msg.data);
                socket.send(zmq::buffer(std::to_string(delete_result)));
                break;
                }
            case STAT:
                {
                socket.send(zmq::buffer(std::to_string(SUCCESS) + GetStat(msg.data)));
                break;  
                }
            case PLAY:
                {
                int flag = 0;
                for(std::pair<std::string, int> waiting_user : waiting_pull){
                    if (flag){
                        break;
                    }
                    if (waiting_user.second == 0){
                        flag = 1; 
                        waiting_pull[waiting_user.first] = available_port;
                        socket.send(zmq::buffer(std::to_string(SUCCESS) + std::to_string(available_port) + '|' + std::to_string(1)));
                        available_port++;
                    }
                }
                if (flag == 0){
                    waiting_pull.emplace(msg.data, 0);
                    socket.send(zmq::buffer(std::to_string(WAIT)));
                }
                }
                break;
            case WAIT:
                {
                if (waiting_pull[msg.data] > 0){
                    socket.send(zmq::buffer(std::to_string(SUCCESS) + std::to_string(waiting_pull[msg.data]) + '|' + std::to_string(0)));
                    waiting_pull.erase(msg.data);
                } else{
                    socket.send(zmq::buffer(std::to_string(WAIT)));                    
                }
                }
                break;
            case WIN:
                socket.send(zmq::buffer(std::to_string(SetStat(msg.data, 1))));
                break;
            case LOSE:
                socket.send(zmq::buffer(std::to_string(SetStat(msg.data, 0))));
                break;
            default:
                socket.send(zmq::buffer(std::to_string(FAIL)));
                break;
        }
    }
}