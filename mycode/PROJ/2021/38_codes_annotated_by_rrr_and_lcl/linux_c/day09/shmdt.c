#include <func.h>
int main()
{
    int shmid = shmget(1000,4096,IPC_CREAT|0600);
    char * p = (char *)shmat(shmid,NULL,0);
    //sleep(5);
    //shmdt(p);
    while(1);
    return 0;
}

