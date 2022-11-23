#include "../include/head.h"

typedef struct{                                                                                                                                      
    int dataLen1;//记录用户名长度
    int dataLen2;//记录密码的长度
    char buf1[1000];
    char buf2[1000];
    }Train_t;

int main(int argc,char*argv[])
{
    int sfd = socket(AF_INET,SOCK_STREAM,0);
    ERROR_CHECK(sfd,-1,"socket");

    //保存本机的IP和端口
    struct sockaddr_in serAddr;
    memset(&serAddr,0,sizeof(serAddr));
    serAddr.sin_family = AF_INET;
    serAddr.sin_addr.s_addr = inet_addr(argv[1]);
    serAddr.sin_port = htons(atoi(argv[2]));

    //用户验证小火车
    Train_t USER_INFO;
    memset(&USER_INFO,0,sizeof(USER_INFO));

    //建立连接
    int ret = 0;
    ret = connect(sfd,(struct sockaddr*)&serAddr,sizeof(serAddr));
    ERROR_CHECK(ret,-1,"connect");

    //登陆验证功能
    while(1){
        //保存用户名、密码
        char name[20], *passwd;
        memset(name,0,sizeof(name));
        passwd = NULL;

        system("clear");

        printf("请输入用户名:\n");
        scanf("%s",name);
        passwd = getpass("请输入密码:");

        //加入小火车
        strcpy(USER_INFO.buf1,name);
        USER_INFO.dataLen1 = strlen(name);
        strcpy(USER_INFO.buf2,passwd);
        USER_INFO.dataLen2 = strlen(passwd);
        
        //发送给服务端
        send(sfd,&USER_INFO,sizeof(USER_INFO),0);

        //接收服务端
        int res;
        memset(&res,0,sizeof(res));

        //收到登录结果
        recv(sfd,&res,10,0);
        if(res == 1){
            printf("用户名密码正确，欢迎登陆！\n");
            break;
        }
        else{
            printf("用户名密码错误，请重试！\n");
            system("pause");
        }
    }
    //进入菜单
        
    return 0;
}
