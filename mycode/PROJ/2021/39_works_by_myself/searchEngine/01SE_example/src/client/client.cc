#include <json/config.h>
#include <json/json.h>
#include <json/reader.h>
#include <json/value.h>

#include <stdlib.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sys/epoll.h>

#include <sstream>

int main(int argc,char* argv[])
{
    if(argc != 3) {
        perror("need ip port");
        exit(1);
    }
    printf("IP: %s  PORT: %s\n", argv[1], argv[2]);
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in service;
    bzero(&service, sizeof(service));
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_addr(argv[1]);
    service.sin_port = htons(atoi(argv[2]));
    int ret;
    ret = connect(socket_fd, (struct sockaddr*)&service, sizeof(service));
    if(ret == -1) {
        return -1;
    }

    // json
    Json::Value root;
    Json::CharReaderBuilder builder;
    builder["collectComments"] = true;
    JSONCPP_STRING errs;
    std::stringstream ss;
    // end of json

    int epfd = epoll_create(1);
    struct epoll_event event, evs[2];
    event.events = EPOLLIN;
    event.data.fd = STDIN_FILENO;
    epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &event);
    event.data.fd = socket_fd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, socket_fd, &event);
    int readyCount;
    char buf[65536] = {0};
    struct sockaddr_in serAddr;
    serAddr.sin_family = AF_INET;
    serAddr.sin_addr.s_addr = inet_addr(argv[1]);
    serAddr.sin_port = ntohs(atoi(argv[2]));
    ret = connect(socket_fd, (struct sockaddr*)&serAddr, sizeof(serAddr));
    size_t len;
    printf("要查询的词: ");
    rewind(stdout);
    while(1)
    {
        readyCount = epoll_wait(epfd, evs, 2, -1);
        if(readyCount != 0)
        {
            for(int i = 0; i != readyCount; ++i)
            {
                if(evs[i].data.fd == STDIN_FILENO)
                {
                    memset(buf, 0, sizeof(buf));
                    ret = read(STDIN_FILENO, buf, sizeof(buf)-1);
                    if(ret == 0)
                    {
                        puts("bye!");
                        goto END;
                    }
                    len = strlen(buf) - 1;
                    send(socket_fd, &len, sizeof(size_t), 0);
                    send(socket_fd, buf, strlen(buf)-1, 0);
                }
                if(evs[i].data.fd)
                {
                    memset(buf, 0, sizeof(buf));
                    ret = recv(socket_fd, &len, sizeof(size_t), 0);
                    if(ret == -1 || ret == 0)
                    {
                        puts("\n对方已断开");
                        goto END;
                    }
                    ret = recv(socket_fd, buf, len, 0);
                    // 解码 并 打印
                    ss << buf;
                    if (!parseFromStream(builder, ss, &root, &errs)) {
                        perror(errs.c_str());
                        return EXIT_FAILURE;
                    }
                    puts("候选词:");
                    if(root.size() != 0) {
                        for(auto k : root) {
                            puts(k.asString().c_str());
                        }
                    } else {
                        puts("NULL");
                    }
                    
                    printf("\n要查询的词: ");
                    rewind(stdout);
                }
            }
        }
    }
END:
    close(socket_fd);
    return 0;
}

