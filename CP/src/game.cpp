#include "game.hpp"

Gamer::Gamer(std::string & port, int initiative) : context{1}, socket{context, zmq::socket_type::pair}{
    if (initiative){
        socket.bind(("tcp://*:" + port).c_str());
    } else{
        socket.connect(("tcp://localhost:" + port).c_str());
    }
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            map[i][j] = 0;
        }
    }
}

void Gamer::PrintMap(){
    std::cout << "Your map" << std::endl;
    std::cout << std::endl;
    std::cout << "   A  B  C  D  E  F  G  H  I  J" << std::endl;
    for(int i = 0; i < 10; i++){
        std::cout << "--------------------------------" << std::endl;
        std::cout << i + 1;
        if (i < 9){
            std::cout << ' ';
        }
        for(int j = 0; j < 10; j++){
            if ((map[i][j] > 0) and (map[i][j] < 5)){
                std::cout << "|X|";
            } else{
                std::cout << "| |";
            }
        }
        std::cout << std::endl;
    }
    std::cout << "--------------------------------" << std::endl;
}

int Gamer::ConvertAlphToInt(char ch){
    if(ch == 'A'){
        return 0;
    } else if(ch == 'B'){
        return 1;
    } else if(ch == 'C'){
        return 2;
    } else if(ch == 'D'){
        return 3;
    } else if(ch == 'E'){
        return 4;
    } else if(ch == 'F'){
        return 5;
    } else if(ch == 'G'){
        return 6;
    } else if(ch == 'H'){
        return 7;
    } else if(ch == 'I'){
        return 8;
    } else if(ch == 'J'){
        return 9;
    } else{
        return -1;
    }
}

int Gamer::InitFieldAround(int str, int col){
    if (str == 0){
        if (col == 0){
            if((map[str][col+1] > 0) and (map[str][col+1] < 5)){
                if(map[str][col+1] == BUSY4){
                    map[str][col] = CANNOT_PLACE;
                    return 0;
                }
                map[str][col] = map[str][col+1] + 1;
                int lim = map[str][col+1];
                for(int i = 1; i <= lim; i++){
                    map[str][col+i]++;
                }
            }
            if((map[str+1][col] > 0) and (map[str+1][col] < 5)){
                if(map[str+1][col] == BUSY4){
                    map[str][col] = CANNOT_PLACE;
                    return 0;
                }
                map[str][col] = map[str+1][col] + 1;
                int lim = map[str+1][col];
                for(int i = 1; i <= lim; i++){
                    map[str+i][col]++;
                }
            }
            map[str+1][col+1] = CANNOT_PLACE; 
        } else if (col == 9){
            if((map[str][col-1] > 0) and (map[str][col-1] < 5)){
                if(map[str][col-1] == BUSY4){
                    map[str][col] = CANNOT_PLACE;
                    return 0;
                }
                map[str][col] = map[str][col-1] + 1;
                int lim = map[str][col-1];
                for(int i = 1; i <= lim; i++){
                    map[str][col-i]++;
                }
            }
            if((map[str+1][col] > 0) and (map[str+1][col] < 5)){
                if(map[str+1][col] == BUSY4){
                    map[str][col] = CANNOT_PLACE;
                    return 0;
                }
                map[str][col] = map[str+1][col] + 1;
                int lim = map[str+1][col];
                for(int i = 1; i <= lim; i++){
                    map[str+i][col]++;
                }
            }
            map[str+1][col-1] = CANNOT_PLACE;
        } else{
            if((map[str][col+1] > 0) and (map[str][col+1] < 5)){
                if(map[str][col+1] == BUSY4){
                    map[str][col] = CANNOT_PLACE;
                    return 0;
                }
                map[str][col] = map[str][col+1] + 1;
                int lim = map[str][col+1];
                for(int i = 1; i <= lim; i++){
                    map[str][col+i]++;
                }
            }
            if((map[str+1][col] > 0) and (map[str+1][col] < 5)){
                if(map[str+1][col] == BUSY4){
                    map[str][col] = CANNOT_PLACE;
                    return 0;
                }
                map[str][col] = map[str+1][col] + 1;
                int lim = map[str+1][col];
                for(int i = 1; i <= lim; i++){
                    map[str+i][col]++;
                }
            }
            if((map[str][col-1] > 0) and (map[str][col-1] < 5)){
                if(map[str][col-1] == BUSY4){
                    map[str][col] = CANNOT_PLACE;
                    return 0;
                }
                map[str][col] = map[str][col-1] + 1;
                int lim = map[str][col-1];
                for(int i = 1; i <= lim; i++){
                    map[str][col-i]++;
                }
            }
            map[str+1][col+1] = CANNOT_PLACE;
            map[str+1][col-1] = CANNOT_PLACE;
        }
    } else if (str == 9){
         if (col == 0){
            if((map[str][col+1] > 0) and (map[str][col+1] < 5)){
                if(map[str][col+1] == BUSY4){
                    map[str][col] = CANNOT_PLACE;
                    return 0;
                }
                map[str][col] = map[str][col+1] + 1;
                int lim = map[str][col+1];
                for(int i = 1; i <= lim; i++){
                    map[str][col+i]++;
                }
            }
            if((map[str-1][col] > 0) and (map[str-1][col] < 5)){
                if(map[str-1][col] == BUSY4){
                    map[str][col] = CANNOT_PLACE;
                    return 0;
                }
                map[str][col] = map[str-1][col] + 1;
                int lim = map[str-1][col];
                for(int i = 1; i <= lim; i++){
                    map[str-i][col]++;
                }
            }
            map[str-1][col+1] = CANNOT_PLACE; 
        } else if (col == 9){
            if((map[str-1][col] > 0) and (map[str-1][col] < 5)){
                if(map[str-1][col] == BUSY4){
                    map[str][col] = CANNOT_PLACE;
                    return 0;
                }
                map[str][col] = map[str-1][col] + 1;
                int lim = map[str-1][col];
                for(int i = 1; i <= lim; i++){
                    map[str-i][col]++;
                }
            }
            if((map[str][col-1] > 0) and (map[str][col-1] < 5)){
                if(map[str][col-1] == BUSY4){
                    map[str][col] = CANNOT_PLACE;
                    return 0;
                }
                map[str][col] = map[str][col-1] + 1;
                int lim = map[str][col-1];
                for(int i = 1; i <= lim; i++){
                    map[str][col-i]++;
                }
            }
            map[str-1][col-1] = CANNOT_PLACE;
        } else{
            if((map[str-1][col] > 0) and (map[str-1][col] < 5)){
                if(map[str-1][col] == BUSY4){
                    map[str][col] = CANNOT_PLACE;
                    return 0;
                }
                map[str][col] = map[str-1][col] + 1;
                int lim = map[str-1][col];
                for(int i = 1; i <= lim; i++){
                    map[str-i][col]++;
                }
            }
            if((map[str][col-1] > 0) and (map[str][col-1] < 5)){
                if(map[str][col-1] == BUSY4){
                    map[str][col] = CANNOT_PLACE;
                    return 0;
                }
                map[str][col] = map[str][col-1] + 1;
                int lim = map[str][col-1];
                for(int i = 1; i <= lim; i++){
                    map[str][col-i]++;
                }
            }
            if((map[str][col+1] > 0) and (map[str][col+1] < 5)){
                if(map[str][col+1] == BUSY4){
                    map[str][col] = CANNOT_PLACE;
                    return 0;
                }
                map[str][col] = map[str][col+1] + 1;
                int lim = map[str][col+1];
                for(int i = 1; i <= lim; i++){
                    map[str][col+i]++;
                }
            }
            map[str-1][col+1] = CANNOT_PLACE;
            map[str-1][col-1] = CANNOT_PLACE;
        }
    } else{
        if((map[str-1][col] > 0) and (map[str-1][col] < 5)){
            if(map[str-1][col] == BUSY4){
                map[str][col] = CANNOT_PLACE;
                return 0;
            }
            map[str][col] = map[str-1][col] + 1;
            int lim = map[str-1][col];
            for(int i = 1; i <= lim; i++){
                map[str-i][col]++;
            }
        }
        if((map[str+1][col] > 0) and (map[str+1][col] < 5)){
            if(map[str+1][col] == BUSY4){
                map[str][col] = CANNOT_PLACE;
                return 0;
            }
            map[str][col] = map[str+1][col] + 1;
            int lim = map[str+1][col];
            for(int i = 1; i <= lim; i++){
                map[str+i][col]++;
            }
        }
         if (col == 0){
            if((map[str][col+1] > 0) and (map[str][col+1] < 5)){
                if(map[str][col+1] == BUSY4){
                    map[str][col] = CANNOT_PLACE;
                    return 0;
                }
                map[str][col] = map[str][col+1] + 1;
                int lim = map[str][col+1];
                for(int i = 1; i <= lim; i++){
                    map[str][col+i]++;
                }
            }
            map[str+1][col+1] = CANNOT_PLACE;
            map[str-1][col+1] = CANNOT_PLACE; 
        } else if (col == 9){
            if((map[str][col-1] > 0) and (map[str][col-1] < 5)){
                if(map[str][col-1] == BUSY4){
                    map[str][col] = CANNOT_PLACE;
                    return 0;
                }
                map[str][col] = map[str][col-1] + 1;
                int lim = map[str][col-1];
                for(int i = 1; i <= lim; i++){
                    map[str][col-i]++;
                }
            }
            map[str+1][col-1] = CANNOT_PLACE;
            map[str-1][col-1] = CANNOT_PLACE;
        } else{
            if((map[str][col-1] > 0) and (map[str][col-1] < 5)){
                if(map[str][col-1] == BUSY4){
                    map[str][col] = CANNOT_PLACE;
                    return 0;
                }
                map[str][col] = map[str][col-1] + 1;
                int lim = map[str][col-1];
                for(int i = 1; i <= lim; i++){
                    map[str][col-i]++;
                }
            }
            if((map[str][col+1] > 0) and (map[str][col+1] < 5)){
                if(map[str][col+1] == BUSY4){
                    map[str][col] = CANNOT_PLACE;
                    return 0;
                }
                map[str][col] = map[str][col+1] + 1;
                int lim = map[str][col+1];
                for(int i = 1; i <= lim; i++){
                    map[str][col+i]++;
                }
            }
            map[str+1][col+1] = CANNOT_PLACE;
            map[str+1][col-1] = CANNOT_PLACE;
            map[str-1][col+1] = CANNOT_PLACE;
            map[str-1][col-1] = CANNOT_PLACE;
        }
    }
    if (map[str][col] == EMPTY){
        map[str][col] = BUSY1;
    }
    return map[str][col];
}

void Gamer::CreateMap(){
    char command;
    std::cout << "Create your map! Enter 'h' for help" << std::endl;
    int deck4_counter = 0, deck3_counter = 0, deck2_counter = 0, deck1_counter = 0;
    int in_editor = 1;
    while(in_editor){
        PrintMap();
        std::cout << "> ";
        std::cin >> command;
        switch (command){
        case 'h':
            std::cout << "h - help" << std::endl;
            std::cout << "p COORDS - place deck. Example: p A1" << std::endl;
            std::cout << "e - erase map" << std::endl;
            std::cout << "l - leave editor" << std::endl;
            break;
        case 'p':
            {
            int str, col;
            char string_col;
            std::cin >> string_col >> str;
            col = ConvertAlphToInt(string_col);
            str--;
            if (map[str][col] == EMPTY){
                int ship_size = InitFieldAround(str, col);
                if (ship_size == 1){
                    deck1_counter++;
                } else if(ship_size == 2){
                    deck1_counter--;
                    deck2_counter++;
                } else if(ship_size == 3){
                    deck2_counter--;
                    deck3_counter++;
                } else if(ship_size == 4){
                    deck3_counter--;
                    deck4_counter++;
                } else{
                    std::cout << "You can't place deck here" << std::endl;  
                }
            } else{
                std::cout << "You can't place deck here" << std::endl;
            }
            }
            break;
        case 'e':
            for(int i = 0; i < 10; i++){
                for(int j = 0; j < 10; j++){
                    map[i][j] = EMPTY;
                }
            }
            break;
        case 'l':
            if ((deck1_counter == 4) and (deck2_counter == 3) and (deck3_counter == 2) and (deck4_counter == 1)){
                in_editor = 0;
            } else{
                std::cout << "Incorrect number of ships. Must be: one-deck - 4, two-deck - 3, three-deck - 2, four-deck - 1" << std::endl;
            }
            break;
        default:
        std::cout << "Invalid input! Enter 'h' for help" << std::endl;
            break;
        }
    }
}

void Gamer::PrintTwoMaps(std::string & enemy_login, int enemy_map[10][10]){
    std::cout << "Your map                           " << enemy_login << " map" << std::endl;
    std::cout << std::endl;
    std::cout << "   A  B  C  D  E  F  G  H  I  J       A  B  C  D  E  F  G  H  I  J" << std::endl;
    for(int i = 0; i < 10; i++){
        std::cout << "--------------------------------   --------------------------------" << std::endl;
        std::cout << i + 1;
        if (i < 9){
            std::cout << ' ';
        }
        for(int j = 0; j < 10; j++){
            if (map[i][j] == BUSY){
                std::cout << "|0|";
            } else if(map[i][j] == POINT){
                std::cout << "|.|";
            } else if(map[i][j] == SUNK){
                std::cout << "|X|";
            } else {
                std::cout << "| |";
            }
        }
        std::cout << "   ";
        std::cout << i + 1;
        if (i < 9){
            std::cout << ' ';
        }
        for(int j = 0; j < 10; j++){
            if (enemy_map[i][j] == BUSY){
                std::cout << "|0|";
            } else if(enemy_map[i][j] == POINT){
                std::cout << "|.|";
            } else if(enemy_map[i][j] == SUNK){
                std::cout << "|X|";
            } else {
                std::cout << "| |";
            }
        }
        std::cout << std::endl;
    }
    std::cout << "--------------------------------   --------------------------------" << std::endl;
}

int Gamer::Attack(int enemy_map[10][10], int * sunk_counter){
    std::cout << "Your turn!" << std::endl;
    zmq::message_t msg;
    char command;
    int attack = 1;
    while(attack){
        std::cout << "> ";
        std::cin >> command;
        switch (command){
            case 'h':
                std::cout << "'h' - help" << std::endl;
                std::cout << "'a COORD' - attack field. Example: a A1" << std::endl;
                std::cout << "'p COORD' - set point in field. Example: p A1" << std::endl;
                std::cout << "'g' - give up" << std::endl;
                break;
            case 'p':
                {
                int str, col;
                char string_col;
                std::cin >> string_col >> str;
                col = ConvertAlphToInt(string_col);
                str--;
                if (enemy_map[str][col] == EMPTY){
                    enemy_map[str][col] = POINT;
                } else{
                    std::cout << "Field busy!" << std::endl;
                }
                }
                break;
            case 'a':
                {
                zmq::message_t reply;
                MSG_ID msg_id;
                 int str, col;
                char string_col;
                std::cin >> string_col >> str;
                col = ConvertAlphToInt(string_col);
                str--;
                socket.send(zmq::buffer(std::to_string(SHOT) + std::to_string(str) + std::to_string(col)));
                socket.recv(reply);
                msg_id = static_cast<MSG_ID>(stoi(reply.to_string()));
                if(msg_id == MISS){
                    std::cout << "MISS!" << std::endl;
                    enemy_map[str][col] = POINT;
                } else if (msg_id == WOUND){
                    std::cout << "WOUNDED!" << std::endl;
                    enemy_map[str][col] = SUNK;
                    *sunk_counter++;
                } else if (msg_id == KILL){
                    std::cout << "KILLED!" << std::endl;
                    enemy_map[str][col] = SUNK;
                    *sunk_counter++;
                }
                attack = 0;
                }   
                break;
            case 'g':
                {
                socket.send(zmq::buffer(std::to_string(GIVE_UP)));
                return 1;
                }
                break;
            default:
                break;
        }
    }
    return 0;
}

int Gamer::KillTest(int str, int col){
    if (str == 0){
        if (col == 0){
            if(map[str][col+1] == BUSY){
                return 0;
            } else if(map[str+1][col] == BUSY){
                return 0;
            }
        } else if (col == 9){
            if(map[str][col-1] == BUSY){
                return 0;
            } else if(map[str+1][col] == BUSY){
                return 0;
            }
        } else{
            if(map[str][col+1] == BUSY){
                return 0;
            } else if(map[str+1][col] == BUSY){
                return 0;
            } else if(map[str][col-1] == BUSY){
                return 0;
            }
        }
    } else if (str == 9){
         if (col == 0){
            if(map[str][col+1] == BUSY){
                return 0;
            } else if(map[str-1][col] == BUSY){
                return 0;
            } 
        } else if (col == 9){
            if(map[str][col-1] == BUSY){
                return 0;
            } else if(map[str-1][col] == BUSY){
                return 0;
            }
        } else{
            if(map[str][col-1] == BUSY){
                return 0;
            } else if(map[str-1][col] == BUSY){
                return 0;
            } else if(map[str][col+1] == BUSY){
                return 0;
            }
        }
    } else{
        if(map[str-1][col] == BUSY){
            return 0;
        } else if(map[str+1][col] == BUSY){
            return 0;
        }
        if (col == 0){
            if(map[str][col+1] == BUSY){
                return 0;
            }
        } else if (col == 9){
            if(map[str][col-1] == BUSY){
                return 0;
            }
        } else{
            if(map[str][col+1] == BUSY){
                return 0;
            } else if(map[str][col-1] == BUSY){
                return 0;
            }
        }
    }
    return 1;
}

int Gamer::Defend(int * sunk_counter){
    std::cout << "Enemy turn!" << std::endl; 
    zmq::message_t msg;
    socket.recv(msg);
    int msg_id = (msg.to_string()[0] - '0') * 10 + (msg.to_string()[1] - '0');
    if (msg_id == GIVE_UP){
        return 1;
    }
    int str = msg.to_string()[2] - '0';
    int col = msg.to_string()[3] - '0';
    if (map[str][col] == BUSY){
        map[str][col] = SUNK;
        if (KillTest(str, col)){
            std::cout << "Your ship was sunk" <<std::endl;
            socket.send(zmq::buffer(std::to_string(KILL)));
            *sunk_counter++;
        } else{
            std::cout << "You were injured" <<std::endl;
            socket.send(zmq::buffer(std::to_string(WOUND)));
            *sunk_counter++;
        }
    } else{
        std::cout << "Enemy missed" <<std::endl;
        socket.send(zmq::buffer(std::to_string(MISS)));
    }
    return 0;
}

int Gamer::Play(int initiative, std::string login){
    std::string enemy_login;
    zmq::message_t msg;
    if(initiative){
        socket.send(zmq::buffer(login));
        socket.recv(msg);
        enemy_login = msg.to_string();
    } else{
        socket.recv(msg);
        enemy_login = msg.to_string();
        socket.send(zmq::buffer(login));
    }
    CreateMap();
    int enemy_map[10][10];
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            enemy_map[i][j] = 0;
            if (map[i][j] > 0 and map[i][j] < 5){
                map[i][j] = BUSY;
            } else{
                map[i][j] = EMPTY;
            }
        }    
    }
    std::cout << "Waiting for the enemy" << std::endl;
    if (initiative){
        zmq::message_t reply;
        socket.send(zmq::buffer(std::to_string(WAIT)));
        socket.recv(reply);
    } else{
        zmq::message_t reply;
        socket.recv(reply);
        socket.send(zmq::buffer(std::to_string(SUCCESS)));
    }
    std::cout << "Enter 'h' for help" << std::endl;
    int res;
    int sunk_counter_my = 0, sunk_counter_enemy = 0;
    while(1){
        PrintTwoMaps(enemy_login, enemy_map);
        if (initiative){
            if(Attack(enemy_map, &sunk_counter_enemy)){
                res = 0;
                break;
            }
            if(sunk_counter_enemy == 20){
                res = 1;
                break;
            }
            PrintTwoMaps(enemy_login, enemy_map);
            if(Defend(&sunk_counter_my)){
                res = 1;
                break;
            }
            if(sunk_counter_my == 20){
                res = 1;
                break;
            }
        } else{
            if(Defend(&sunk_counter_my)){
                res = 1;
                break;
            }
            if(sunk_counter_my == 20){
                res = 1;
                break;
            }
            PrintTwoMaps(enemy_login, enemy_map);
            if(Attack(enemy_map, &sunk_counter_enemy)){
                res = 0;
                break;
            }
            if(sunk_counter_enemy == 20){
                res = 1;
                break;
            }
        }
    }
    if (res){
        std::cout << "You win" << std::endl;
    } else{
        std::cout << "You lose" << std::endl;
    }
    return res;
}