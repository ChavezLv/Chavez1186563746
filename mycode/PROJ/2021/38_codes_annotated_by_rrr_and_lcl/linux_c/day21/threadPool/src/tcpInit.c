#include "../include/head.h"

int tcpInit(char *ip, char *port, int *sockFd)
{
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(sfd, -1, "socket");

    struct sockaddr_in serAddr;
    memset(&serAddr, 0, sizeof(serAddr));
    serAddr.sin_family = AF_INET;
    serAddr.sin_addr.s_addr = inet_addr(ip);
    serAddr.sin_port = htons(atoi(port));

    //设置套接口地址可重用
    int reuse = 1; // 变量大于0，设置的选项生效
    setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    int ret = 0;
    ret = bind(sfd, (struct sockaddr*)&serAddr, sizeof(serAddr));
    ERROR_CHECK(ret, -1, "bind");

    ret = listen(sfd, 10);
    ERROR_CHECK(ret, -1, "listen");

    *sockFd = sfd;

    return 0;

}
