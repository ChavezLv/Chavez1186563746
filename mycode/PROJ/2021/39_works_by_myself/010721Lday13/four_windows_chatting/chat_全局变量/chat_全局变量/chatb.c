#include <func.h>

typedef struct Msg_s{
    int m_type;
    char buf[4096];
}Msg_t,*pMsg_t;

typedef struct Pid_s{
    pid_t pid[4];
}Pid_t,*pPid_t;
pid_t pid;
pPid_t pId = NULL;
pMsg_t tmp = NULL;
void newFunc(int signum,siginfo_t *p,void* q){
    printf("sig:%d\n",signum);
    for(int i = 0;i<4;++i){
        if(pId->pid[i]!=pid){
            kill(pId->pid[i],9);
        }
    }
    shmdt(pId);
    shmdt(tmp);
    kill(pid,9);
}
int main(int argc,char*argv[]){
    pid = getpid();
    int shmid = shmget(1024,1<<21,IPC_CREAT|0600);
    ERROR_CHECK(shmid,-1,"shmget");
    tmp = (pMsg_t)shmat(shmid,NULL,0);
    ERROR_CHECK(tmp,(void*)-1,"shmat");

    int shmid_pid = shmget(1025,1<<10,IPC_CREAT|0600);
    ERROR_CHECK(shmid_pid,-1,"shmget");
    pId = (pPid_t)shmat(shmid_pid,NULL,0);
    ERROR_CHECK(pId,(void*)-1,"shmat");
    pId->pid[3] = pid;

    int semid = semget(1000,1,IPC_CREAT|0600);
    semctl(semid,0,SETVAL,1); 
    
    struct sembuf p,v;
    p.sem_num = 0;
    p.sem_op = -1;
    p.sem_flg = 0;
    v.sem_num = 0;
    v.sem_op = 1;
    v.sem_flg = 0;

    struct sigaction sig;
    memset(&sig,0,sizeof(sig));
    sig.sa_sigaction = newFunc;
    sig.sa_flags = SA_SIGINFO; 
    int ret = sigaction(10,&sig,NULL); 
    ERROR_CHECK(ret,-1,"sigaction");

    /* sigset_t set; */
    /* sigemptyset(&set); */
    /* sigaddset(&set,2); */
    /* sigprocmask(SIG_SETMASK,&set,NULL); */
    
    /* sigset_t sigPending; */
    /* ret = sigpending(&sigPending); */
    while(1){
        if(tmp->m_type == 1){
            semop(semid,&p,1);
            printf("recv:%s",tmp->buf);
            fflush(stdout);
            memset(tmp,0,sizeof(Msg_t));
            semop(semid,&v,1);
            printf("pid:%d\n",pId->pid[3]);
            printf("pid:%d\n",pId->pid[2]);
        }else if(tmp->m_type == 2){
            semop(semid,&p,1);
            printf("send:%s",tmp->buf);
            fflush(stdout);
            memset(tmp,0,sizeof(Msg_t));
            semop(semid,&v,1);
            printf("pid:%d\n",pId->pid[3]);
            printf("pid:%d\n",pId->pid[2]);
            kill(pId->pid[2],9);
        }
        /* if(sigismember(&sigPending,2)){ */
        /*     printf("sigPending.\n"); */
        /*     sigprocmask(SIG_UNBLOCK,&set,NULL); */
        /*     break; */
        /* } */    
    }
    shmdt(tmp);
    return 0;
}

