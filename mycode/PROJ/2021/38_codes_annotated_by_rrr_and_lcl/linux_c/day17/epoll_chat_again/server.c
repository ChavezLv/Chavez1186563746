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

    //epoll_create参数填一个大于零的值，表示创建
    int epfd = epoll_create(1);
    ERROR_CHECK(epfd, -1, "epoll_create");

    struct epoll_event event, evs[2];
    memset(&event, 0, sizeof(event));

    event.events = EPOLLIN;
    event.data.fd = STDIN_FILENO;

    // 把文件描述符加入到epfd中
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &event);
    ERROR_CHECK(ret, -1, "epoll_ctl");

    event.data.fd = sfd;

    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, sfd, &event);
    ERROR_CHECK(ret, -1, "epoll_ctl");

    char buf[64] = {0};

    int readyNum = 0;

    while(1){
        //监控就绪的文件描述符
        readyNum  = epoll_wait(epfd, evs, 2, -1);
        ERROR_CHECK(readyNum, -1, "epoll_wait");
        for(int i = 0; i < readyNum; ++i){
            if(evs[i].data.fd == STDIN_FILENO){
                memset(buf, 0, sizeof(buf));
                read(STDIN_FILENO, buf, sizeof(buf));
                send(newFd, buf, strlen(buf)-1, 0);
            }
            if(evs[i].data.fd == newFd){
                memset(buf, 0, sizeof(buf));
                int ret = recv(newFd, buf, sizeof(buf), 0);
                if(0 == ret){
                    printf("client exit\n");
                    close(newFd);
                    continue;
                }

                printf("buf = %s\n", buf);
            }
            //监听sfd，用于接收新的连接
            if(evs[i].data.fd == sfd){
                newFd = accept(sfd, NULL, NULL);
                event.data.fd = newFd;
                epoll_ctl(epfd, EPOLL_CTL_ADD, newFd, &event);
            }
        }
    }

    close(sfd);
    close(newFd);
    return 0;
}

