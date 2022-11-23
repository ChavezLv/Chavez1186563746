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

    int newFd = 0;
    ERROR_CHECK(newFd, -1, "accept");

    fd_set rdset;
    fd_set needMonitorset;
    FD_ZERO(&needMonitorset);
    FD_ZERO(&rdset);

    FD_SET(sfd, &needMonitorset);
    FD_SET(STDIN_FILENO, &needMonitorset);

    char buf[64] = {0};

    int readyNum = 0;

    while(1){
        memcpy(&rdset, &needMonitorset, sizeof(fd_set));
        //select的第二个参数是一个传入传出参数，每次遍历完，都会修改集合的内容
        //所以需要定义一个新的集合，防止需要监听的描述符被修改
        readyNum = select(10 , &rdset, NULL, NULL, NULL);
        
        ERROR_CHECK(readyNum, -1, "select");
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
                    printf("client exit\n");
                    close(newFd);
                    FD_CLR(newFd, &needMonitorset);
                    continue;
                }

                printf("buf = %s\n", buf);
            }
            //监听sfd，用于接收新的连接
            if(FD_ISSET(sfd, &rdset)){
                newFd = accept(sfd, NULL, NULL);
                FD_SET(newFd, &needMonitorset);

            }
        }
    }

    close(sfd);
    close(newFd);
    return 0;
}

