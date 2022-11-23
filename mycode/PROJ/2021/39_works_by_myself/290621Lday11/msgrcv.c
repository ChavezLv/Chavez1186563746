#include <func.h>

typedef struct MsgBuf_s{
    long type;
    char content[10];
}Msgbuf_t;

int main(int argc,char*argv[]){
    ARGS_CHECK(argc,2);
    Msgbuf_t msgr = {0};
    memset(&msgr,0,sizeof(Msgbuf_t));
    msgr.type = atoi(argv[1]);

    int msgid = msgget(1024,IPC_CREAT|0666);
    ERROR_CHECK(msgid,-1,"msgget");
    int ret = msgrcv(msgid,&msgr,sizeof(msgr.content),msgr.type,0);
    ERROR_CHECK(ret,-1,"msgrcv");
    printf("retrcv:%d.\n",ret);
    puts(msgr.content);
    return 0;
}

