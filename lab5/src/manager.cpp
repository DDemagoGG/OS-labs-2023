#include "manager.hpp"

int main (int argc, char const *argv[]){
	char * path_to_computer = getenv("PATH_TO_COMPUTER");
	Tree t(-1);
	std::map<int, Socket *> children;
	std::string command;
	Msg new_msg;
	Msg reply;
	BindingSocket * new_socket;
	while(std::cin >> command){
		if (command == "create"){
			int parent_id, child_id;
			std::cin >> child_id >> parent_id;
			if (t.find(child_id)){
				std::cout << "Error: Already exists" << std::endl;
				continue;
			}
			if (t.find(parent_id) == nullptr){
				std::cout << "Error: Parent not found" << std::endl;
				continue;
			}
			new_msg.setMsgArgs(parent_id, child_id);
			if (parent_id == -1){
				new_socket = new BindingSocket(child_id);
				children.emplace(child_id, new_socket);
				int child_pid = fork();
				if (child_pid == 0){
					execl(path_to_computer, path_to_computer, std::to_string(child_id).c_str(), nullptr);
					return 0;
				} else{
					new_msg.action = connect;
					if (children[child_id]->sendRecvMsg(&new_msg, reply) and reply.action == success){
						t.push(-1, child_id);
					} else{
						children.erase(child_id);
					}
				}
			} else{
				int root_child = t.findRootChild(parent_id);
				new_msg.action = create;
				new_msg.setMsgArgs(parent_id, child_id);
				if (children[root_child]->sendNoWaitRecvMsg(&new_msg, reply) and reply.action == success){
					t.push(parent_id, child_id);
				} else{
					std::cout << "Error: Parent is unavailable" << std::endl;
				}
			}
		} else if (command == "exec"){
			int id;
			std::string key;
			int value;
			std::cin >> id >> key;
			if (t.find(id) == nullptr){
				std::cout << "Error:id: Not found" << std::endl;
				continue;
			}
			key += '#';
			if(getchar() == ' '){
				new_msg.action = add;
				std::cin >> value;
			} else{
				new_msg.action = check;
			}
			int root_child = t.findRootChild(id);
			if (new_msg.action == check){
				new_msg.arg2 = id;
				for (auto i: key){
					new_msg.arg1 = i;
					if (!children[root_child]->sendNoWaitRecvMsg(&new_msg, reply) or reply.action == fail){
						std::cout << "Error:id: Node is unavailable" << std::endl;
					}
				}
			} else if (new_msg.action == add){
				new_msg.arg2 = id;
				for (auto i: key){
					new_msg.arg1 = i;
					if (i == '#'){
						new_msg.arg2 = value;
					}
					if (!children[root_child]->sendNoWaitRecvMsg(&new_msg, reply) or reply.action == fail){
						std::cout << "Error:id: Node is unavailable" << std::endl;
					}
				}
			}
		} else if (command == "pingall"){
			new_msg.action = ping;
			std::vector<int> busy_processes;
			for(auto p : children){
				if(p.second->sendNoWaitRecvMsg(&new_msg, reply)){
					while (reply.action == busy){
						p.second->sendMsg(&new_msg);
						busy_processes.push_back(reply.arg1);
						p.second->recvMsg(reply);
					}
				} else{
					t.find(p.first)->traversal(busy_processes);
				}
			}
			std::cout << "Ok: ";
			if (busy_processes.size() > 0){
				for (auto i : busy_processes){
					std::cout << i << ';';
				}
			} else{
				std::cout << -1;
			}
			std::cout << std::endl;
		} else if (command == "print"){
			t.printTree();
		} else{
			std::cout << "Invalid command" << std::endl;
		}
	}
	new_msg.action = die;
	for(auto p : children){
		p.second->sendRecvMsg(&new_msg, reply);
	}
	std::cout << "bye!" << std::endl;
	return 0;
}