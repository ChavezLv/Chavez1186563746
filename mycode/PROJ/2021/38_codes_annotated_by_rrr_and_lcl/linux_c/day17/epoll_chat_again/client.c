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

    int ret = 0;

    ret = connect(sfd, (struct sockaddr*)&serAddr, sizeof(serAddr));
    ERROR_CHECK(ret, -1, "connect");

    fd_set rdset;
    FD_ZERO(&rdset);

    char buf[64] = {0};

    int readyNum = 0;

    while(1){
        FD_SET(STDIN_FILENO, &rdset);
        FD_SET(sfd, &rdset);
        readyNum = select(sfd + 1, &rdset, NULL, NULL, NULL);
        for(int i = 0; i < readyNum; ++i){
            if(FD_ISSET(STDIN_FILENO, &rdset)){
                memset(buf, 0, sizeof(buf));
                read(STDIN_FILENO, buf, sizeof(buf));
                send(sfd, buf, strlen(buf)-1, 0);
            }
            if(FD_ISSET(sfd, &rdset)){
                memset(buf, 0, sizeof(buf));
                recv(sfd, buf, sizeof(buf), 0);
                printf("buf = %s\n", buf);
            }
        }
    }

    close(sfd);
    return 0;
}

