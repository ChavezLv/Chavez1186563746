#include <head.h>

int main(int argc, char **argv)
{
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(sfd, -1, "socket");

    //保存本机的ip和端口
    struct sockaddr_in serAddr;
    memset(&serAddr, 0, sizeof(serAddr));
    serAddr.sin_family = AF_INET;
    serAddr.sin_addr.s_addr = inet_addr(argv[1]);
    serAddr.sin_port = htons(atoi(argv[2]));

    //绑定本机的IP和端口，绑定到sfd上
    int ret = 0;
    ret = bind(sfd, (struct sockaddr*)&serAddr, sizeof(serAddr));

    //监听的最大连接数是10
    ret = listen(sfd, 10);
    ERROR_CHECK(ret, -1, "listen");

    //接收连接,返回新的文件描述符，新的描述符作用，与对端进行数据交互使用的
    int newFd = accept(sfd, NULL, NULL);
    ERROR_CHECK(newFd, -1, "accept");

    //创建epoll,参数必须大于0
    int epfd = epoll_create(1);
    ERROR_CHECK(epfd, -1, "epoll_create");

    struct epoll_event events, evs[2];
    memset(&events, 0, sizeof(events));
    //告诉内核监听读事件
    events.events = EPOLLIN; 
    events.data.fd = STDIN_FILENO;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &events);
    ERROR_CHECK(ret, -1, "epoll_ctl");

	//对于需要监听的文件描述符只需要添加一次
    events.events = EPOLLIN; 
    events.data.fd = newFd;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, newFd, &events);
    ERROR_CHECK(ret, -1, "epoll_ctl");


    char buf[4] = {0};
    int readyNum = 0;
    while(1){
        readyNum = epoll_wait(epfd, evs, 2, -1);
        printf("after wait\n");
        for(int i = 0; i < readyNum; ++i){

            //如果是标准输入就绪，就代表需要发送数据给对端
            if(evs[i].data.fd == STDIN_FILENO){
                memset(buf, 0, sizeof(buf));
                read(STDIN_FILENO, buf, sizeof(buf)); 
                send(newFd, buf, strlen(buf)-1, 0);
            }

            //如果是newFd就绪，就说明对端有数据发送给我们
            if(evs[i].data.fd == newFd){
                memset(buf, 0, sizeof(buf));
                ret = recv(newFd, buf, sizeof(buf)-1, 0);
                if(0 == ret){
                    /* close(newFd); */
                send(newFd, "bey", 3, 0);
                    printf("bey bey\n");
                    continue;
                }
                printf("buf = %s\n", buf);
            }
        }
    }
    close(sfd);
    close(newFd);

    return 0;
}

