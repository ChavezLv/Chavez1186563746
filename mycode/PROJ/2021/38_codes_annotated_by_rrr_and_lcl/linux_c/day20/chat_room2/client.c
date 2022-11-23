/*************************************************************************
	> File Name: main.c
	> Author: Name
	> Mail: Name@163.com 
	> Created Time: 2020-06-12 11:45:27
 ************************************************************************/

#include <head.h>

int main(int argc,char* argv[])
{
    int sfd;
    sfd = socket(AF_INET,SOCK_STREAM,0);

    struct sockaddr_in serAddr;
    serAddr.sin_family = AF_INET;
    serAddr.sin_addr.s_addr = inet_addr(argv[1]);
    serAddr.sin_port = htons(atoi(argv[2]));

    int ret = 0;
    ret = connect(sfd,(struct sockaddr*)&serAddr,sizeof(serAddr));
    ERROR_CHECK(ret,-1,"connect");

    char buf[64]={0};
    fd_set rdset;
    while(1)
    {
        FD_ZERO(&rdset);
        FD_SET(STDIN_FILENO,&rdset);
        FD_SET(sfd,&rdset);
        select(10,&rdset,NULL,NULL,NULL);
        if(FD_ISSET(STDIN_FILENO,&rdset))
        {
            //标准输入可读，读出数据，发给对方
            memset(buf,0,sizeof(buf));
            read(STDIN_FILENO,buf,sizeof(buf));
            send(sfd,buf,strlen(buf)-1,0);
        }
        if(FD_ISSET(sfd,&rdset))
        {
            //如果newFd可读，代表对方发数据给我们
            //读出数据，打印
            memset(buf,0,sizeof(buf));
            recv(sfd,buf,sizeof(buf),0);
            printf("%s\n",buf);
        }

    }

    close(sfd);
    return 0;
 
}
