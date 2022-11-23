#include <func.h>

typedef struct Msg_s{
    int type;
    char buf[4096];
}Msg_t;
typedef struct Pid_s{
    /* int flag; */
    int sig; 
    /* pid_t pid[4]; */
}Pid_t,*pPid_t;
/* void sigFunc(int signum){ */
/*     printf("sig:%d\n",signum); */
/* } */
int main(int argc,char*argv[]){
        
    /* int ret ; */
    int msgid = msgget(1000,IPC_CREAT|0600);
    ERROR_CHECK(msgid,-1,"msgget");

    Msg_t msg;

    int shmid_pid = shmget(1025,1<<10,IPC_CREAT|0600);
    ERROR_CHECK(shmid_pid,-1,"shmget");
    pPid_t pId = (pPid_t)shmat(shmid_pid,NULL,0);
    ERROR_CHECK(pId,(void*)-1,"shmat");
    /* pId->flag = 0;//首次启动 */
    pId->sig = -1;//设定初始值为-1
    
    signal(10,NULL);

    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set,10);
    sigprocmask(SIG_SETMASK,&set,NULL);
    
    sigset_t pend;
    while(1){
        sigpending(&pend);
        /* printf("%d\n",pId->sig); */
        if(sigismember(&pend,10)||pId->sig == 10){
            pId->sig = 10;
            break;
        }    
        //会堵塞住以至于收不到信号
        memset(&msg,0,sizeof(msg));
        msgrcv(msgid,&msg,sizeof(msg),msg.type,IPC_NOWAIT);
        /* ERROR_CHECK(ret,-1,"msgrcv"); */

        if(msg.type == 1){
            printf("recv:%s",msg.buf);
            fflush(stdout);
        }else if(msg.type == 2){
            printf("send:%s",msg.buf);
            fflush(stdout);
        }
    }
    shmdt(pId);
    sigprocmask(SIG_UNBLOCK,&set,NULL);
    return 0;
}

