#include <iostream>
int fork_process();
void create_pipe(int* pipe_fd);
void dup_fd(int oldfd, int newfd);
void processes(std::istream &input);