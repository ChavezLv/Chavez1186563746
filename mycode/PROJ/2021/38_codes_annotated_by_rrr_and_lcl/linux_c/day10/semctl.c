#include <func.h>
#define N 1000000
int main()
{
    int semid = semget(2000,1,IPC_CREAT|0600);
    ERROR_CHECK(semid,-1,"semget");
    int ret = semctl(semid,0,SETVAL,1);//1-【SETVAL】: 直接拔信号量数组的某一个信号量设置值（顺便会把历史的增减也清除）
    ERROR_CHECK(ret,-1,"semctl SETVAL");//2-用【GETVAL】:则可以读出信号量的值
    struct sembuf P;
    P.sem_num = 0;//下标
    P.sem_op = -1;//P操作会减少可用资源
    P.sem_flg = SEM_UNDO;
    struct sembuf V;
    V.sem_num = 0;
    V.sem_op = 1;//V操作会释放可用资源
    V.sem_flg = SEM_UNDO;

    int shmid = shmget(1000,4096,IPC_CREAT|0600);
    int *p = (int *)shmat(shmid,NULL,0);
    p[0] = 0;
    struct timeval beg,end;
    gettimeofday(&beg,NULL);
    if(fork() == 0){
        for(int i = 0; i < N; ++i){
            semop(semid,&P,1);//1表示总共操作1个信号量值
            p[0]++;
            semop(semid,&V,1);
        }
        exit(0);
    }
    else{
        for(int i = 0; i < N; ++i){
            semop(semid,&P,1);
            p[0]++;
            semop(semid,&V,1);
        }
        wait(NULL);
        gettimeofday(&end,NULL);
        long timeTotal = (end.tv_sec - beg.tv_sec)*1000000 + end.tv_usec - beg.tv_usec;
        printf("Total = %d, timeTotal = %ld, timeAvg = %lf\n", p[0], timeTotal, (double )timeTotal/4000000);
        exit(0);
    }
}

int main()
{
    int semid = semget(2000,2,IPC_CREAT|0600);
    ERROR_CHECK(semid,-1,"semget");
    unsigned short arr[] = {3,5};
    semctl(semid,0,SETALL,arr);//此时第2个参数无效 //3-【SETALL】可以设置在数组里，一口气设置所有信号量
    //int val1 = semctl(semid,0,GETVAL);
    //int val2 = semctl(semid,1,GETVAL);//4-【GETALL】可以一口气用数组接住所有的信号量
    //printf("val1 = %d, val2 = %d\n",val1,val2);
    unsigned short ret[2];
    semctl(semid,0,GETALL,ret);
    for(int i = 0; i < 2; ++i){
        printf("ret[%d] = %d\n",i,ret[i]);
    }
}

int main()
{
    int semid = semget(2000,2,IPC_CREAT|0600);
    ERROR_CHECK(semid,-1,"semget");
    struct semid_ds statbuf;
    semctl(semid,0,IPC_STAT, &statbuf);
    printf("perm = %o\n",statbuf.sem_perm.mode);
    printf("cuid = %d\n",statbuf.sem_perm.cuid);
    printf("nsems = %ld\n",statbuf.sem_nsems);
    statbuf.sem_perm.mode = 0666;
    semctl(semid,0,IPC_SET,&statbuf);//设置参数
    semctl(semid,0,IPC_RMID);//删除信号量
}


