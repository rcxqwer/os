#include "kernel/types.h"
#include "user/user.h"

// 判断左管道有无数据
int lpipe_first(int lpipe[2], int *data)
{
  if (read(lpipe[0], data, sizeof(int)) == sizeof(int)) {
    printf("prime %d\n", *data);
    return 0;
  }
  return -1;
}

// 往右管道中传输数据
void transmit_data(int lpipe[2], int rpipe[2], int first)
{
  int data;
  // 从左管道读取数据
  while (read(lpipe[0], &data, sizeof(int)) == sizeof(int)) {
    // 过滤数据
    if (data % first)
      write(rpipe[1], &data, sizeof(int));
  }
  close(lpipe[0]);
  close(rpipe[1]);
}

//  过滤管道中的非素数
void primes(int lpipe[]){
    close(lpipe[1]);

    int first;
    if (lpipe_first(lpipe, &first) == 0){
        // 创建当前的管道
        int p[2];
        pipe(p);
        // 往管道中传输数据
        transmit_data(lpipe, p, first);
        
        // 子进程过滤数据
        if (fork() == 0) {
            primes(p);   
        } else {
            close(p[0]);
            wait(0);
        }
    }
    // 关闭线程
    exit(0);
}

int main(int argc, char const *argv[])
{   
    // 创建初始管道
    int p[2];
    pipe(p);

    // 写入初始数据
    for (int i = 2; i <= 35; i++){
        write(p[1], &i, sizeof(int));
    }
    
    // 子进程过滤数据
    if (fork() == 0){
        primes(p);
    }
    else{
        close(p[0]);
        close(p[1]);
        wait(0);
    }
    
    exit(0);

    return 0;
}
