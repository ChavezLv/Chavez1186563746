#include <head.h>

int main(int argc, char** argv)
{
    int sfd = socket(AF_INET, SOCK_DGRAM, 0);//协议：udp数据报
    //我怀疑sfd的端口天然就可以接收数据，那bind到底有什么必要呢？
    ERROR_CHECK(sfd, -1, "socket");

    struct sockaddr_in serAddr; //即使是udp依然要实现【把自己绑定一个对外输出的端口】这一步
    memset(&serAddr, 0, sizeof(serAddr));
    serAddr.sin_family = AF_INET;
    serAddr.sin_addr.s_addr = inet_addr(argv[1]);
    serAddr.sin_port = htons(atoi(argv[2]));

    int ret = 0;

    socklen_t serLen = sizeof(serAddr);
    char buf[64] = {0};
    fd_set rdset;
    FD_ZERO(&rdset);

    int readyNum = 0;

    while(1){
        FD_SET(STDIN_FILENO, &rdset);
        FD_SET(sfd, &rdset);
        readyNum = select(sfd + 1, &rdset, NULL, NULL, NULL);
        for(int i = 0; i < readyNum; ++i){
            if(FD_ISSET(STDIN_FILENO, &rdset)){//键盘响应发送
                memset(buf, 0, sizeof(buf)); 
                read(STDIN_FILENO, buf, sizeof(buf));
                sendto(sfd, buf, strlen(buf)-1, 0, (struct sockaddr*)&serAddr, serLen);
            }
            if(FD_ISSET(sfd, &rdset)){//受到对端显示打印
                memset(buf, 0, sizeof(buf));
                recvfrom(sfd, buf, sizeof(buf)-1, 0, (struct sockaddr*)&serAddr, &serLen);
                printf("buf = %s\n", buf);
            }
        }
    }
    close(sfd);
    return 0;
}

