#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char const *argv[]) {
    int pid;
    int buffer[64] = {0};
    int parent_fd[2];
    int child_fd[2];

    // 建立管道
    pipe(parent_fd); 
    pipe(child_fd);

    if ((pid = fork()) == 0){
        // 子进程
        // 关闭父进程的写端，并且读出数据
        close(parent_fd[1]);
        read(parent_fd[0], buffer, 4);

        printf("%d: received %s\n", getpid(), buffer);

        // 关闭子进程的读端, 并且写入数据 
        close(child_fd[0]);
        write(child_fd[1], "pong", sizeof(buffer));
        exit(0);
    }else {
        // 父进程
        close(parent_fd[0]);
        write(parent_fd[1], "ping", sizeof(buffer));

        close(child_fd[1]);
        read(child_fd[0], buffer, 4);
        printf("%d: received %s\n", getpid(), buffer);
        exit(0);
    }
}
