#include <func.h>

typedef struct MsgBuf_s{
    long type;
    char content[10];
}Msgbuf_t;
int main(int argc,char*argv[]){
    int msgid = msgget(1024,IPC_CREAT|0666);
    ERROR_CHECK(msgid,-1,"msgget");
    
    ARGS_CHECK(argc,3);
    Msgbuf_t msgBuf = {0};
    msgBuf.type = atoi(argv[1]);
    strcpy(msgBuf.content,argv[2]);
    int ret = msgsnd(msgid,&msgBuf,sizeof(msgBuf.content),0);
    ERROR_CHECK(ret,-1,"msgsnd");
    printf("retsnd:%d.\n",ret);
    return 0;
}

