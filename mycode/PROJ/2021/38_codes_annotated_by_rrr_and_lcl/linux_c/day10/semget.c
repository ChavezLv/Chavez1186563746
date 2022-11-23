#include <func.h>
int main()
{
    int semid = semget(2000,2,IPC_CREAT|0600);
    ERROR_CHECK(semid,-1,"semget");
    while(1);
    return 0;
}

