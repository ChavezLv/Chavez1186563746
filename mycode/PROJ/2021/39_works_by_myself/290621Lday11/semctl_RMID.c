#include <func.h>

int main(int argc,char*argv[]){
    int semid  = semget(3000,2,IPC_CREAT|0600);
    ERROR_CHECK(semid,-1,"semget");
    
    int ret;
    /* unsigned char arr[] = {0,5}; */
    /* ret = semctl(semid,0,SETALL,arr); */
    /* ERROR_CHECK(ret, -1, "semctl"); */

    //直接删除
    ret = semctl(semid, 0, IPC_RMID, NULL); 
    ERROR_CHECK(ret, -1, "semctl");
    return 0;
}

