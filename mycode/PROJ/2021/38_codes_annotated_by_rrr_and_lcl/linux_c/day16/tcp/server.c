#include <head.h>

int main(int argc, char** argv)
{
    //sfd用于与对端建立连接
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(sfd, -1, "socket");

    //结构体保存本地的ip和port
    struct sockaddr_in serAddr;
    memset(&serAddr, 0, sizeof(serAddr));
    serAddr.sin_family = AF_INET;
    serAddr.sin_addr.s_addr = inet_addr(argv[1]);
    serAddr.sin_port = htons(atoi(argv[2]));

    int ret = 0;
    //bind把ip和port与sfd进行绑定
    ret = bind(sfd, (struct sockaddr*)&serAddr, sizeof(serAddr));
    ERROR_CHECK(ret, -1, "bind");

    //监听对端的连接请求，第二个参数是同时监听的最大连接数
    ret = listen(sfd, 10);
    ERROR_CHECK(ret, -1, "listen");
    
    //accept函数返回的newFd是用于建立连接之后服务端和客户端进行数据交互使用的
    int newFd = accept(sfd, NULL, NULL);
    ERROR_CHECK(newFd, -1, "accept");

    char buf[64] = {0};

    send(newFd, "helloserver", 11, 0);

    recv(newFd, buf, sizeof(buf)-1, 0);
    printf("buf = %s\n", buf);

    close(sfd);
    return 0;
}

