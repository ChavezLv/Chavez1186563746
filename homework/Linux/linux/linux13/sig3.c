#include<func.h>
struct sigaction act2;
//void sigfunc1(int sigNum){       
//        printf("interrupt1 is coming!\n");
//        sleep(5);
//        sigpending(&act2.sa_mask);
//        if( sigismember(&act2.sa_mask,SIGQUIT)){
//            printf("there is int3 in int2\n");
//        }
//    }
void sigfunc2(int sigNum){
        printf("interrupt2 is coming!\n");
    }
int main(int argc,char** argv){
    memset(&act2,0,sizeof(act2));
    act2.sa_handler=sigfunc2;
    sigemptyset(&act2.sa_mask);//清空屏蔽字
    sigset_t newmask,oldmask,pendmask;
    //  sigaddset(&act2.sa_mask,SIGQUIT);
    sigaddset(&newmask,SIGQUIT);
    act2.sa_flags=0;
    act2.sa_flags|=SA_RESTART;
    int ret2=sigaction(SIGQUIT,&act2,NULL);
    sigprocmask(SIG_BLOCK,&newmask,&oldmask);
    sleep(5);
    sigpending(&pendmask);
    if(sigismember(&pendmask,SIGQUIT)){
        printf("there are blocked signals!");
    }
    sigprocmask(SIG_SETMASK,&oldmask,NULL);
    
    char buf[1024]={0};
    read(STDIN_FILENO,buf,sizeof(buf));
    if(errno==EINTR){
        perror("read STDIN_FILRNO");

    }
    puts(buf);
    return 0;
}
