#include <func.h>
typedef struct{
    long mtype;
    char mtext[256];
} myMsg_t;
int main()
{
    int msgid = msgget(1000,IPC_CREAT|0600);
    ERROR_CHECK(msgid, -1, "msgget");
    myMsg_t msg;
    memset(&msg,0,sizeof(msg));
    msgrcv(msgid,&msg,sizeof(msg.mtext),3,0);
    printf("type = %ld, msg = %s\n",msg.mtype,msg.mtext);
    return 0;
}

