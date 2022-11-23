#include <func.h>
typedef struct {
    long mtype;
    char mtext[256];
} myMsg_t;//消息的结构需要自己定义
int main()
{
    int msgid = msgget(1000,IPC_CREAT|0600);
    ERROR_CHECK(msgid, -1, "msgget");
    myMsg_t msg;
    msg.mtype = 1;//这个类型是任一正数
    memset(msg.mtext,0,sizeof(msg.mtext));
    strcpy(msg.mtext,"hello");
    msgsnd(msgid,&msg,5,0);
    return 0;
}

