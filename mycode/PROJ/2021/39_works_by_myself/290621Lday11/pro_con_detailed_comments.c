#include <func.h>
/*
 *最后产品不变的原因是:一消费完就生产上去了
 * */
int main(int argc, char **argv)
{
    int ret = 0;
    //semget创建创建信号量集,int semget(key_t key, int nsems, int semflg);
    //semaphore set identifier associated with the argument key
    //nsems会指定信号集合的信号量数量,须大于0;semflg位掩码,IPC_CREAT|IPC_EXCEL
    int semid = semget(3000, 2, IPC_CREAT|0666);
    ERROR_CHECK(semid, -1, "semget");

    //union semun {
    //    int              val;    /* Value for SETVAL */
    //    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    //    unsigned short  *array;  /* Array for GETALL, SETALL */
    //    struct seminfo  *__buf;  /* Buffer for IPC_INFO
    //                                (Linux-specific) */
    //};
    //Array for GETALL, SETALL
    //如上,是结构体的设计, 类型需要是unsigned short arr,否则会在调用semop时卡住
    //0代表商品的数量，5代表货架的数量
    unsigned short arr[2] = {0, 5};

    //IPCE_RMID/ GETVAL需要4个参数
    //int semctl(int semid, int semnum, int cmd, ...);
    //0表示所有semnum
    //将semid指定的信号量集中第n个信号量转换为arg.val
    semctl(semid, 0, SETALL, arr);//把需要控制的资源放入SETALL里面

    struct sembuf sop[2];
    memset(sop, 0, sizeof(sop));

    //父进程充当生产者
    if(fork()){
        //生产者生产商品，消耗货架
        sop[0].sem_num = 0; //表示商品
        sop[0].sem_op = 1;//对商品+1
        sop[0].sem_flg = 0;

        sop[1].sem_num = 1;
        sop[1].sem_op = -1;
        sop[1].sem_flg = 0;
        while(1){
            //int semop(int semid, struct sembuf *sops, size_t nsops);
            //sops是一个指向数组的指针,数组中包含了需要执行的操作,nsops为数组大小,操作按数组数组以原子方式执行
            ret = semop(semid, sop, 2);
            ERROR_CHECK(ret, -1, "semop1");
            //semctl(semid, 0, GETVAL)返回商品对应的资源量的值
            printf("生产者：商品的数量 = %d, 货架的数量 = %d\n", semctl(semid, 0, GETVAL), semctl(semid, 1, GETVAL));
            sleep(1);
        }
    }else{
        //消费者，消耗商品，释放货架
        sop[0].sem_num = 0;
        sop[0].sem_op = -1;
        sop[0].sem_flg = 0;

        sop[1].sem_num = 1;
        sop[1].sem_op = 1;
        sop[1].sem_flg = 0;

        while(1){
            ret = semop(semid, sop, 2);
            ERROR_CHECK(ret, -1, "semop2");

            printf("消费者：商品的数量 = %d, 货架的数量 = %d\n", semctl(semid, 0, GETVAL), semctl(semid, 1, GETVAL));
            sleep(2);
        }
    }
    return 0;
}


