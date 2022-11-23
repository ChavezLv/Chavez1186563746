#include <func.h>
int main()
{/*生产者消费者v1：仅仅实现【临界区互斥访问】，而并不在意【同步先后关系】，比如【有货品才能卖/有空位才能产】*/
//你稍等一下，这个函数也有检查，明天确认一下到底会不会出错
//获取信号量
    int semid = semget(1000,1,IPC_CREAT|0600);    
    semctl(semid,0,SETVAL,1);
//定义操作 
    struct sembuf P;
    P.sem_num = 0;
    P.sem_op = -1;
    P.sem_flg = SEM_UNDO;
    struct sembuf V;
    V.sem_num = 0;
    V.sem_op = 1;
    V.sem_flg = SEM_UNDO;
//获得【资源】【共享空间】
    int shmid = shmget(1000,4096,IPC_CREAT|0600);
    int *p = (int *)shmat(shmid,NULL,0);
    p[0] = 5;//商品个数
    p[1] = 5;//空间个数

    if(fork() == 0){
        //子进程消费
        while(1){
            sleep(1);//消费者消费的频率更快
            printf("before consume! p[0] = %d, p[1] = %d\n",p[0],p[1]);
            semop(semid,&P,1);//互斥锁
            //----临界区
            printf("beg\n");
            sleep(5);
            printf("over\n");
            if(p[0] > 0){
                --p[0];//消费行为
                ++p[1];
                printf("shopping! p[0] = %d, p[1] = %d\n",p[0],p[1]);
            }
            //----临界区结束
            semop(semid,&V,1);//互斥锁解锁
            printf("after consume! p[0] = %d, p[1] = %d\n",p[0],p[1]);
        }
    }
    else{
        //父进程生产
        while(1){
            sleep(2);
            printf("before produce! p[0] = %d, p[1] = %d\n",p[0],p[1]);
            semop(semid,&P,1);
            //----互斥锁
            if(p[1] > 0){
                ++p[0];
                --p[1];//应该是这里被打断，啊！！！所以泥鳅说的这里的恢复是【恢复互斥信号量】所以并不会出问题
                printf("producing! p[0] = %d, p[1] = %d\n",p[0],p[1]);
            }
            //----互斥锁解锁
            semop(semid,&V,1);
            printf("after produce! p[0] = %d, p[1] = %d\n",p[0],p[1]);
        }
    }
    return 0;
}

