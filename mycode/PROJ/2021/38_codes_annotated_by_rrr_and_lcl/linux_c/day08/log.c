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
    for(int i = 0; i < 10; ++i){
        time_t now;
        time(&now);
        struct tm *pTm = gmtime(&now);
        syslog(LOG_INFO,"%d %04d:%02d:%02d",i,pTm->tm_year+1900,pTm->tm_mon+1,pTm->tm_mday);
        sleep(2);
    }
    return 0;
}

