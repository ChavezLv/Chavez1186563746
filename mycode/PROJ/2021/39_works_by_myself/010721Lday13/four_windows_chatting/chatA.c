#include <func.h>

typedef struct Msg_s{
    int type;
    char buf[4096];
}Msg_t;

typedef struct Pid_s{
    int flag;
    int sig; 
    /* pid_t pid[4]; */
}Pid_t,*pPid_t;
void sigFunc(int signum){
    printf("sig:%d\n",signum);
}
int main(int argc,char*argv[]){
    ARGS_CHECK(argc,3);
    int ret;
    int fdr = open(argv[1],O_RDONLY);
    ERROR_CHECK(fdr,-1,"open");
    int fdw = open(argv[2],O_WRONLY);
    ERROR_CHECK(fdw,-1,"open");

    fd_set fdrSet;
    Msg_t msg;
    memset(&msg,0,sizeof(msg));
    int msgid = msgget(1000,IPC_CREAT|0600);
    ERROR_CHECK(msgid,-1,"msgget");
    
    int shmid_pid = shmget(1025,1<<10,IPC_CREAT|0600);
    ERROR_CHECK(shmid_pid,-1,"shmget");
    pPid_t pId = (pPid_t)shmat(shmid_pid,NULL,0);
    ERROR_CHECK(pId,(void*)-1,"shmat");
    pId->flag = 0;//首次启动
    if(pId->sig == 10){
        pId->sig = -1;
    }
    
    signal(10,NULL);

    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set,10);
    sigprocmask(SIG_SETMASK,&set,NULL);
    
    sigset_t pend;
    while(1){
        FD_ZERO(&fdrSet);
        FD_SET(fdr,&fdrSet);
        FD_SET(STDIN_FILENO,&fdrSet);
        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 10;
        ret = select(fdw+1,&fdrSet,NULL,NULL,&timeout);
        ERROR_CHECK(ret,-1,"select");

        sigpending(&pend);
        if(sigismember(&pend,10)||pId->sig == 10){
            printf("%d\n",pId->sig);
            pId->sig = 10;
            break;
        }    
        if(FD_ISSET(fdr,&fdrSet)){
            memset(&msg,0,sizeof(msg));
            ret = read(fdr,msg.buf,sizeof(msg.buf));
            /* ERROR_CHECK(ret,-1,"read"); */
            ERROR_CHECK(ret,0,"read");
            /* printf("%s\n",msg.buf); */
            msg.type = 1;
            ret = msgsnd(msgid,&msg,sizeof(msg),0);
            ERROR_CHECK(ret,-1,"msgsnd");
        }
        if(FD_ISSET(STDIN_FILENO,&fdrSet)){
            memset(&msg,0,sizeof(msg));
            ret = read(STDIN_FILENO,msg.buf,sizeof(msg.buf));
            /* printf("%d\n",ret); */
            ERROR_CHECK(ret,0,"read");
            write(fdw,msg.buf,ret);
            /* sprintf(tmp,"%s.\n",buf); */
            /* printf("%s\n",msg.buf); */
            msg.type = 2;
            ret = msgsnd(msgid,&msg,sizeof(msg),0);
            ERROR_CHECK(ret,-1,"msgsnd");
        }
    }
    close(fdr);
    close(fdw);
    return 0;
}

