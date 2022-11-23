#include <func.h>
#define NUM 100000
int main()
{
    int shmid = shmget(IPC_PRIVATE,4096,IPC_CREAT|0600);
    //IPC_PRIVATE说明这个共享内存是私有的//只有内存之间可见
    ERROR_CHECK(shmid,-1,"shmget");
    int *p = (int *)shmat(shmid,NULL,0);
    ERROR_CHECK(p,(void *)-1,"shmat");
    p[0] = 0;
    if(fork() == 0){//子进程
        for(int i = 0; i < NUM; ++i){
            ++p[0];
        }
        exit(0);
    }
    else{
        for(int i = 0; i < NUM; ++i){
            ++p[0];
        }
        wait(NULL);
        printf("Total = %d\n",p[0]);
        exit(0);
    }
}

