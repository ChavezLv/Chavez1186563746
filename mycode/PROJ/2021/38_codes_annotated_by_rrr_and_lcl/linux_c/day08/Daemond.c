#include <func.h>
#define MAXFD 64
void Daemon(){
    if(fork() != 0){
        exit(0);
    }
    setsid();//子进程创建一个新的会话
    for(int i = 0; i < MAXFD; ++i){
        close(i);
    }
    chdir("/");
    umask(0);
}
int main()
{
    Daemon();
    while(1){
        sleep(1);
    }
    return 0;
}

