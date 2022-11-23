#include <func.h>

typedef struct Msg_s{
    int type;
    char buf[4096];
}Msg_t;

int main(int argc,char*argv[]){
    int ret ;
    int msgid = msgget(1000,IPC_CREAT|0600);
    ERROR_CHECK(msgid,-1,"msgget");

    Msg_t msg;

    while(1){
        memset(&msg,0,sizeof(msg));
        ret = msgrcv(msgid,&msg,sizeof(msg),msg.type,0);
        ERROR_CHECK(ret,-1,"msgrcv");
        if(msg.type){
            printf("recv:%s",msg.buf);
            fflush(stdout);
        }else{
            printf("send:%s",msg.buf);
            fflush(stdout);
        }
    }
    return 0;
}

