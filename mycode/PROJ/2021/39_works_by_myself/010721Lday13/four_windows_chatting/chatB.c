#include <func.h>
typedef struct Msg_s{
    int m_type;
    char buf[4096];
}Msg_t,*pMsg_t;

typedef struct Pid_s{
    int sig; 
    /* pid_t pid[4]; */
}Pid_t,*pPid_t;
Pid_t pidset;
pid_t pid = {0};
pPid_t pId = NULL;
pMsg_t tmp = NULL;
int fd_exit;
void newFunc(int signum,siginfo_t *p,void* q){
    printf("sig:%d\n",signum);
    /* write(fd_exit,&signum,sizeof(int)); */
}
int main(int argc,char*argv[]){
    ARGS_CHECK(argc,3);
    int ret;
    int fdw = open(argv[1],O_WRONLY);
    ERROR_CHECK(fdw,-1,"open");
    int fdr = open(argv[2],O_RDONLY);
    ERROR_CHECK(fdr,-1,"open");
    
    //退出:共享内存中捕获信号
    /* fd_exit = open(argv[3],O_RDWR); */
    int shmid_pid = shmget(1025,1<<10,IPC_CREAT|0600);
    ERROR_CHECK(shmid_pid,-1,"shmget");
    pId = (pPid_t)shmat(shmid_pid,NULL,0);
    ERROR_CHECK(pId,(void*)-1,"shmat");
    //如果!= 10
    if(pId->sig == 10){
        pId->sig = -1;
    }

    fd_set fdrSet;
    Msg_t msg;

    int shmid = shmget(1024,1<<21,IPC_CREAT|0600);
    ERROR_CHECK(shmid,-1,"shmget");
    tmp = (pMsg_t)shmat(shmid,NULL,0);
    ERROR_CHECK(tmp,(void*)-1,"shmat");

    int semid = semget(1000,1,IPC_CREAT|0600);
    ERROR_CHECK(semid,-1,"semget");
    ret = semctl(semid,0,SETVAL,1); 
    ERROR_CHECK(ret,-1,"semctl");
    
    struct sembuf p,v;
    p.sem_num = 0;
    p.sem_op = -1;
    p.sem_flg = 0;
    v.sem_num = 0;
    v.sem_op = 1;
    v.sem_flg = 0;

    struct sigaction sig;
    sig.sa_sigaction = newFunc;
    sig.sa_flags = SA_SIGINFO; 
    ret = sigaction(10,&sig,NULL);
    ERROR_CHECK(ret,-1,"sigaction");
    
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
        ret = select(fdr+4,&fdrSet,NULL,NULL,&timeout);
        /* printf("ret:%d",ret); */
        if(FD_ISSET(fdr,&fdrSet)){
            memset(&msg,0,sizeof(msg));
            ret = read(fdr,msg.buf,sizeof(msg.buf));
            ERROR_CHECK(ret,0,"read");
            /* printf("%s\n",msg.buf); */
            msg.m_type = 1;
            semop(semid,&p,1);
            /* memset(&tmp,0,sizeof(tmp)); */
            memset(tmp,0,sizeof(Msg_t));
            tmp->m_type = msg.m_type;
            sprintf(tmp->buf,"%s",msg.buf);
            /* printf("%s\n",tmp->buf); */
            semop(semid,&v,1);
        }
        if(FD_ISSET(STDIN_FILENO,&fdrSet)){
            memset(&msg,0,sizeof(msg));
            ret = read(STDIN_FILENO,msg.buf,sizeof(msg.buf));
            /* ERROR_CHECK(ret,-1,"read"); */
            ERROR_CHECK(ret,0,"read");
            msg.m_type = 2;
            /* printf("%s\n",msg.buf); */
            semop(semid,&p,1); 
            memset(tmp,0,sizeof(Msg_t));
            tmp->m_type = msg.m_type;
            strcpy(tmp->buf,msg.buf);
            /* sprintf(tmp->buf,"%s\n",msg.buf); */
            semop(semid,&v,1);
            write(fdw,msg.buf,ret);
        }
        ret = sigpending(&pend);
        ERROR_CHECK(ret , -1 ,"sigpending");
        if(sigismember(&pend,10)||pId->sig == 10){
            pId->sig = 10;
            break;
        }
    }
    shmdt(pId);
    shmdt(tmp);
    close(fdr);
    close(fdw);
    sigprocmask(SIG_UNBLOCK,&set,NULL);
    return 0;
}
