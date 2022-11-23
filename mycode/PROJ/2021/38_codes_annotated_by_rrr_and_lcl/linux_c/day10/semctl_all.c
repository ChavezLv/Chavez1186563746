#include <func.h>
#define N 1000000
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

