#include <func.h>

int main(int argc,char*argv[]){
    int semid = semget(1024,1,IPC_CREAT|0600);
    ERROR_CHECK(semid,-1,"shmget");
    
    int ret = semctl(semid,0,SETVAL,1);
    ERROR_CHECK(ret,-1,"semctl");

    struct sembuf sp,sv;
    //p
    sp.sem_num = 0;
    sp.sem_op = -1;
    sp.sem_flg = SEM_UNDO;
    //v
    sv.sem_num = 0;
    sv.sem_op = 1;
    sv.sem_flg = SEM_UNDO;
    
    int shmid = shmget(1097,4096,IPC_CREAT|0666);
    ERROR_CHECK(shmid,-1,"shmget");
    int *p = (int*)shmat(shmid,NULL,0);
    ERROR_CHECK(p,(void*)-1,"shmat");
    p[0] = 0;
    p[1] = 10;
    if(fork()){
        //consumer
        while(1){
            sleep(2);
            if(p[0]>0){
                ret = semop(semid,&sp,1);
                ERROR_CHECK(ret,-1,"semop");
                --p[0];
                ++p[1];
                printf("sell->commodity:%d,space:%d.\n",p[0],p[1]);
                ret = semop(semid,&sv,1);
                ERROR_CHECK(ret,-1,"semop");
            }
        }
    }else{
        while(1){
            sleep(1);
            if(p[1]>0){
                ret = semop(semid,&sp,1);
                ERROR_CHECK(ret,-1,"semop");
                --p[1];
                ++p[0];
                printf("product->commodity:%d,space:%d.\n",p[0],p[1]);
                ret = semop(semid,&sv,1);
                ERROR_CHECK(ret,-1,"semop");
            }
        }
        wait(NULL);
    }
    return 0;
}

