#include <head.h>

int main(int argc, char** argv)
{
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(sfd, -1, "socket");

    struct sockaddr_in serAddr;
    memset(&serAddr, 0, sizeof(serAddr));
    serAddr.sin_family = AF_INET;
    serAddr.sin_addr.s_addr = inet_addr(argv[1]);
    serAddr.sin_port = htons(atoi(argv[2]));

    //设置套接口地址可重用
    int reuse = 1; // 变量大于0，设置的选项生效
    setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    int ret = 0;
    ret = bind(sfd, (struct sockaddr*)&serAddr, sizeof(serAddr));
    ERROR_CHECK(ret, -1, "bind");

    ret = listen(sfd, 10);
    ERROR_CHECK(ret, -1, "listen");

    int newFd = accept(sfd, NULL, NULL);
    ERROR_CHECK(newFd, -1, "accept");

    fd_set rdset;
    FD_ZERO(&rdset);

    char buf[64] = {0};

    int readyNum = 0;

    while(1){
        //将需要监听的文件描述符加入到集合当中
        FD_SET(STDIN_FILENO, &rdset);
        FD_SET(newFd, &rdset);
        readyNum = select(newFd + 1, &rdset, NULL, NULL, NULL);
        for(int i = 0; i < readyNum; ++i){
            if(FD_ISSET(STDIN_FILENO, &rdset)){
                memset(buf, 0, sizeof(buf));
                read(STDIN_FILENO, buf, sizeof(buf));
                send(newFd, buf, strlen(buf)-1, 0);
            }
            if(FD_ISSET(newFd, &rdset)){
                memset(buf, 0, sizeof(buf));
                int ret = recv(newFd, buf, sizeof(buf), 0);
                if(0 == ret){
                    printf("client error\n");
                    close(sfd);
                    close(newFd);
                    return 0;

                }
                printf("buf = %s\n", buf);
            }
        }
    }

    close(sfd);
    close(newFd);
    return 0;
}

