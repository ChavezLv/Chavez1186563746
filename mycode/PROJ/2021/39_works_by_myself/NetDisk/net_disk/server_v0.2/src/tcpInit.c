#include "../include/head.h"
#include "../include/thread_pool.h"

int tcpInit(char *ip, char *port, int *sockFd){
    int sfd = 0;
    int ret = 0;
    sfd = socket(AF_INET,SOCK_STREAM,0);
    ERROR_CHECK(sfd, -1, "socket");
    
    //设置地址可以重用
    int reuse = 1;
    ret = setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));

    struct sockaddr_in serAddr;
    memset(&serAddr, 0, sizeof(serAddr));
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(atoi(port));
    serAddr.sin_addr.s_addr = inet_addr(ip);

    ret = bind(sfd, (struct sockaddr*)& serAddr, sizeof(struct sockaddr));
    ERROR_CHECK(ret, -1 ,"bind");
    
    ret = listen(sfd, 10);
    ERROR_CHECK(ret, -1, "listen");
    
    *sockFd = sfd;//
    
    return 0;
}

