#include <func.h>
#define N 10000000

int main(int argc,char*argv[]){
    int shmid = shmget(1024,4,IPC_CREAT|0666);
    ERROR_CHECK(shmid,-1,"shmget");
    int *sum = (int*)shmat(shmid,NULL,0);
    ERROR_CHECK(sum,(void*)-1,"shmar");
    
    memset(sum,0,sizeof(int));//clear 

    int semid = semget(1001,1,IPC_CREAT|0666);
    ERROR_CHECK(semid,-1,"semget");
    
    int ret = semctl(semid,0,SETVAL,1);
    ERROR_CHECK(ret,-1,"semctl");
    
    struct sembuf sp,sv;
    //p操作
    sp.sem_num = 0;
    sp.sem_op = -1;
    sp.sem_flg = SEM_UNDO;
    //v操作
    sv.sem_num = 0;
    sv.sem_op = 1;
    sv.sem_flg = SEM_UNDO;
    time_t start,end;
    if(fork()){
        time(&start);
        for(int i = 0;i < N;++i){
            ret = semop(semid,&sp,1);
            ERROR_CHECK(ret,-1,"semop");
            ++(*sum);
            ret = semop(semid,&sv,1);
            ERROR_CHECK(ret,-1,"semop");
        } 
        wait(NULL);//要在子进程结束之后打印
        time(&end);
        printf("sum = %d, cost time: %ld.\n",*sum,end-start);
    }else{
        for(int i = 0;i < N;++i){
            ret = semop(semid,&sp,1);
            ERROR_CHECK(ret,-1,"semop");
            ++(*sum);
            ret = semop(semid,&sv,1);
            ERROR_CHECK(ret,-1,"semop");
        } 
    }
    shmdt(sum);
    return 0;
}

