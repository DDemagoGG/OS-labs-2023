#include "client.hpp"

std::string Start(zmq::socket_t & socket){
    std::string login;
    std::cout << "Welcome to the game The Sea Battle!\nEnter your login to be able to monitor your statistics." << std::endl;
    msg_t msg;
    while(1){
        std::cout << "Login: ";
        std::cin >> login;
        if((login.length() < 5) or (login.length() > 15)){
            std::cout << "login's lenght must be 5-15 symbols. Enter login again." << std::endl; 
        } else{
            zmq::message_t reply;
            msg.id = CONNECT;
            msg.data = login;
            socket.send(zmq::buffer(std::to_string(msg.id) + msg.data), zmq::send_flags::none);
            std::cout << "Connecting with server..." << std::endl;
            socket.recv(reply, zmq::recv_flags::none);
            msg = reply.to_string(); 
            if(msg.id == ONLINE){
                std::cout << "User with this login is already in the game!" << std::endl;
            } else if(msg.id == SUCCESS){
                std::cout << "Congratulations! You are logged in." << std::endl;
                return login;
            } else if(msg.id == FAIL){
                std::cout << "Something went wrong. Try again." << std::endl;
            }
        }

    }
}

void PrintCommandList(){
    std::cout << "'h' - help" << std::endl;
    std::cout << "'p' - play game" << std::endl;
    std::cout << "'s' - view statistics" << std::endl;
    std::cout << "'q' - quit" << std::endl;
}

int QuitGame(zmq::socket_t & socket, std::string & login){
    msg_t msg{QUIT, login};
    zmq::message_t reply;
    socket.send(zmq::buffer(std::to_string(msg.id) + msg.data), zmq::send_flags::none);
    socket.recv(reply, zmq::recv_flags::none);
    msg = reply.to_string();
    if (msg.id == SUCCESS){
        std::cout << "See you later!" << std::endl;
        return 0;
    } else{
        std::cout << "Something went wrong. Try again." << std::endl;
        return 1;
    }
}

void GetStatistics(zmq::socket_t & socket, std::string & login){
    msg_t msg{STAT, login};
    zmq::message_t reply;
    socket.send(zmq::buffer(std::to_string(msg.id) + msg.data), zmq::send_flags::none);
    socket.recv(reply, zmq::recv_flags::none);
    msg = reply.to_string();
    if (msg.id == SUCCESS){
        std::string wins, loses;
        int flag = 1;
        for(char c : msg.data){
            if (c == '|'){
                flag = 0;
                continue;
            }
            if (flag){
                wins.push_back(c);
            } else{
                loses.push_back(c);
            }
        }
        std::cout << "wins: " << wins << std::endl;
        std::cout << "loses: " << loses << std::endl;
    } else{
        std::cout << "Something went wrong. Try again." << std::endl;
    }
}

void Play(zmq::socket_t & main_socket, std::string & login){ 
    msg_t msg{PLAY, login};
    zmq::message_t reply;
    main_socket.send(zmq::buffer(std::to_string(msg.id) + msg.data), zmq::send_flags::none);
    main_socket.recv(reply, zmq::recv_flags::none);
    msg = reply.to_string();
    while(msg.id == WAIT){
        std::cout << "Search for the enemy..." << std::endl;
        sleep(1);
        msg.data = login;
        main_socket.send(zmq::buffer(std::to_string(msg.id) + msg.data), zmq::send_flags::none);
        main_socket.recv(reply, zmq::recv_flags::none);
        msg = reply.to_string();
    }
    if (msg.id == SUCCESS){
        std::string port;
        int initiative;
        int flag = 1;
        for(char c : msg.data){
            if (c == '|'){
                flag = 0;
            }
            if (flag){
                port.push_back(c);
            } else{
                initiative = c - '0';
            }
        }
        Gamer gamer(port, initiative);
        int res = gamer.Play(initiative, login);
        msg.data = login;
        if (res){
            msg.id = WIN;
        } else{
            msg.id = LOSE;
        }
        main_socket.send(zmq::buffer(std::to_string(msg.id) + msg.data));
        main_socket.recv(reply);
        msg = reply.to_string();
        if (msg.id == SUCCESS){
            std::cout << "game results saved" << std::endl;
        }
    }
}

void Routine(){
    zmq::context_t context{1};
    zmq::socket_t socket{context, zmq::socket_type::req};
    socket.connect(("tcp://localhost:" + std::to_string(PORT)).c_str());
    std::string login = Start(socket);
    std::cout << "Enter 'h' for help" << std::endl;
    char command;
    int in_game = 1;
    while (in_game){
        std::cout << "> ";
        std::cin >> command;
        switch (command){
            case 'h':
                PrintCommandList();
                break;
            case 'q':
                in_game = QuitGame(socket, login);
                break;
            case 's':
                GetStatistics(socket, login);
                break;
            case 'p':
                Play(socket, login);
                break;    
            default:
                std::cout << "Invalid input! Enter 'h' for help" << std::endl;
                break;
        }
    }
    
}

int main(){
    Routine();
    return 0;
}