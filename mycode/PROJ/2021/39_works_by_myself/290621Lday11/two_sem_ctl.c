#include <func.h>
/*
 *consume: 7 3 
product: 8 2 
product: 9 1 
consume: 8 2 
product: 9 1 
product: 10 0 
consume: 9 1 
product: 10 0 
consume: 10 0 
product: 10 0 
consume: 10 0 
product: 10 0 
 *最后不变的原因是:一消费完就生产上去了
 * */
int main(int argc,char*argv[]){
    //union semun {
    //    int              val;    /* Value for SETVAL */
    //    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    //    unsigned short  *array;  /* Array for GETALL, SETALL */
    //    struct seminfo  *__buf;  /* Buffer for IPC_INFO
    //                                (Linux-specific) */
    //};
    //如上,是结构体的设计, 类型需要是unsigned short arr,否则会在调用semop时卡住
    unsigned short arr[2] = {0,10};
    
    int semid = semget(1024,2,IPC_CREAT|0600);
    ERROR_CHECK(semid,-1,"semid");

    int ret = semctl(semid,0,SETALL,arr);
    ERROR_CHECK(ret,-1,"semctl");

    struct sembuf sop[2];
    memset(sop,0,sizeof(sop));
    if(fork()){
        sop[1].sem_num = 1;
        sop[1].sem_op = -1;
        sop[1].sem_flg = 0;
        
        sop[0].sem_num = 0;
        sop[0].sem_op = 1;
        sop[0].sem_flg = 0;
        while(1){
            ret = semop(semid,sop,2);
            ERROR_CHECK(ret,-1,"semop");
            
            printf("product: %d %d \n",semctl(semid,0,GETVAL),semctl(semid,1,GETVAL));
            sleep(1);
        }
    }else{
        sop[1].sem_num = 1;
        sop[1].sem_op = 1;
        sop[1].sem_flg = 0;

        sop[0].sem_num = 0;
        sop[0].sem_op = -1;
        sop[0].sem_flg = 0;
        while(1){
            ret = semop(semid,sop,2);
            ERROR_CHECK(ret,-1,"semop");
            printf("consume: %d %d \n",semctl(semid,0,GETVAL),semctl(semid,1,GETVAL));
            sleep(2);
        }
    }
    return 0;
}

