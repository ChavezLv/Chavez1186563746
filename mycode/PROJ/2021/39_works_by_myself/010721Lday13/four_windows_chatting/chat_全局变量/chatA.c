#include <func.h>

typedef struct Msg_s{
    int type;
    char buf[4096];
}Msg_t;

int main(int argc,char*argv[]){
    ARGS_CHECK(argc,3);
    int ret;
    int fdr = open(argv[1],O_RDONLY);
    ERROR_CHECK(fdr,-1,"open");
    int fdw = open(argv[2],O_WRONLY);
    ERROR_CHECK(fdw,-1,"open");

    fd_set fdrSet;
    /* char buf[4096] = {0}; */
    Msg_t msg;
    memset(&msg,0,sizeof(msg));
    int msgid = msgget(1000,IPC_CREAT|0600);
    ERROR_CHECK(msgid,-1,"msgget");
    /* int shmid = shmget(1024,1<<21,IPC_CREAT|0600); */
    /* ERROR_CHECK(shmid,-1,"shmget"); */
    /* char *tmp = (char *)shmat(shmid,NULL,0); */
    /* ERROR_CHECK(buf,(void*)-1,"shmat"); */
    while(1){
        FD_ZERO(&fdrSet);
        FD_SET(fdr,&fdrSet);
        FD_SET(STDIN_FILENO,&fdrSet);
        ret = select(fdw+1,&fdrSet,NULL,NULL,0);
        ERROR_CHECK(ret,-1,"select");
        if(FD_ISSET(fdr,&fdrSet)){
            memset(&msg,0,sizeof(msg));
            ret = read(fdr,msg.buf,sizeof(msg.buf));
            /* ERROR_CHECK(ret,-1,"read"); */
            ERROR_CHECK(ret,0,"read");
            /* printf("%s\n",msg.buf); */
            msg.type = 1;
            ret = msgsnd(msgid,&msg,sizeof(msg),0);
            ERROR_CHECK(ret,-1,"msgsnd");
        }
        if(FD_ISSET(STDIN_FILENO,&fdrSet)){
            memset(&msg,0,sizeof(msg));
            ret = read(STDIN_FILENO,msg.buf,sizeof(msg.buf));
            /* printf("%d\n",ret); */
            ERROR_CHECK(ret,0,"read");
            write(fdw,msg.buf,ret);
            /* sprintf(tmp,"%s.\n",buf); */
            /* printf("%s\n",msg.buf); */
            msg.type = 0;
            ret = msgsnd(msgid,&msg,sizeof(msg),0);
            ERROR_CHECK(ret,-1,"msgsnd");
        }
    }
    return 0;
}

