#include <func.h>

int main(int argc,char*argv[]){
    int shmid  = shmget(1000,4096,IPC_CREAT|0666);
    ERROR_CHECK(shmid,-1,"shmget");

    //attach
    sleep(3);
    char *p = (char*)shmat(shmid,NULL,0); 
    ERROR_CHECK(p,(void*)-1,"shmat");
    sprintf(p,"how are you.\n");
    printf("%s\n",p);

    //shmdt之后要对指针置空,否则会成为悬空指针
    sleep(3);
    shmdt(p);
    p = NULL;
    printf("after shmdt: %s\n",p);

    // IPC_RMID  IPC_STAT  IPC_SET 
    // last change  last shmat  last shmdt
    struct shmid_ds buf;
    int ret = shmctl(shmid,IPC_STAT,&buf);
    ERROR_CHECK(ret,-1,"shmctl");
    printf("%s %s %s\n",ctime(&buf.shm_ctime),ctime(&buf.shm_atime),ctime(&buf.shm_dtime));

    //删除共享内存
    sleep(6);
    shmctl(shmid,IPC_RMID,NULL);
    p = (char*)shmat(shmid,NULL,0); 
    ERROR_CHECK(p,(void*)-1,"shmat");
    return 0;
}

