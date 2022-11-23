#include <func.h>
int main()
{
    /*生产者消费者v2：
    1-【临界区互斥访问】：依靠sem_op数组的【原子性】，把【减少货品+增加空位】连锁起来
    2-【同步先后关系】：直接【用信号量的值代表资源】，所以 【在增加，和，减少】资源之前就有【原子性在一起的检查】
    */
    //申请2个信号量的信号量数组：[0]是【商品】，[1]是【空位】
    int semid = semget(1001,2,IPC_CREAT|0600);
    unsigned short val[] = {5,5};
    semctl(semid,0,SETALL,val);//设定商品和空位的初始值都是5
    //sembuf是【行为的数组】：
    /*1-其实【把空位-1和商品+1】原子化，有一个好处，就是：
        ----最后恢复【阻塞之前的操作】的时候，是【原子化的恢复】，会空位-1和商品+1一起做
        ----如果非原子化，比如说【在商品-1】这个霸占期间死了，之后就会一股脑把商品+1，并不会在意空位
        //这个如果之后看到再研究一次 
        */
    struct sembuf consume[2];//消费行为：【商品】-1，【空位】+1
    consume[0].sem_num = 0;//修改的是商品-1
    consume[0].sem_op = -1;
    consume[0].sem_flg = SEM_UNDO;//每一个都指定了SEM_UNDO,所以每一个都会被撤销，和原子性没有关系

    consume[1].sem_num = 1;//修改的是空格+1
    consume[1].sem_op = 1;
    consume[1].sem_flg = SEM_UNDO;
    
    struct sembuf produce[2];//生产行为：【商品】-1
    produce[0].sem_num = 0;//修改的是商品
    produce[0].sem_op = 1;
    produce[0].sem_flg = SEM_UNDO;
    produce[1].sem_num = 1;//修改的是空格
    produce[1].sem_op = -1;
    produce[1].sem_flg = SEM_UNDO;
    
    if(fork() == 0){
        //子进程消费
        while(1){
            sleep(1);
            printf("before shop, 0 = %d, 1 = %d\n",semctl(semid,0,GETVAL), semctl(semid,1,GETVAL));
            //semop(semid,consume,2);//semop会将修改两个信号量值的操作变为原子操作
            semop(semid,&consume[0],1);//在这里还真的分开了
            printf("beg\n");
            sleep(5);
            printf("over\n");
            semop(semid,&consume[1],1);
            printf("after shop, 0 = %d, 1 = %d\n",semctl(semid,0,GETVAL), semctl(semid,1,GETVAL));
        }
    }
    else{
        //父进程生产
        while(1){
            sleep(2);
            unsigned short ret[2];
            semctl(semid,0,GETALL,ret);
            printf("before produce, 0 = %d, 1 = %d\n",ret[0],ret[1]);
            semop(semid,produce,2);
            semctl(semid,0,GETALL,ret);
            printf("after produce, 0 = %d, 1 = %d\n",ret[0],ret[1]);
        }
    }
    return 0;
}
