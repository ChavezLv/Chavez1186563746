#include <head.h>

int main(int argc, char** argv)
{
    int sfd = socket(AF_INET, SOCK_DGRAM, 0);
    ERROR_CHECK(sfd, -1, "socket");

    struct sockaddr_in serAddr;
    memset(&serAddr, 0, sizeof(serAddr));
    serAddr.sin_family = AF_INET;
    serAddr.sin_addr.s_addr = inet_addr(argv[1]);
    serAddr.sin_port = htons(atoi(argv[2]));

    int ret = 0;
    ret = bind(sfd, (struct sockaddr*)&serAddr, sizeof(serAddr));//SOCKET多了一步bind
    ERROR_CHECK(ret, -1, "bind");

    struct sockaddr_in cliAddr;
    memset(&cliAddr, 0, sizeof(cliAddr));

    socklen_t cliLen = sizeof(cliAddr);
    char buf[64] = {0};(struct sockaddr*)&cliAddr, &cliLen);
    recvfrom(sfd, buf, sizeof(buf)-1, 0, 
    printf("buf = %s\n", buf);

    fd_set rdset;
    FD_ZERO(&rdset);
    int readyNum = 0;

    while(1){
        FD_SET(STDIN_FILENO, &rdset);
        FD_SET(sfd, &rdset);
        readyNum = select(sfd + 1, &rdset, NULL, NULL, NULL);
        for(int i = 0; i < readyNum; ++i){
            if(FD_ISSET(STDIN_FILENO, &rdset)){
                memset(buf, 0, sizeof(buf));
                read(STDIN_FILENO, buf, sizeof(buf));
                sendto(sfd, buf, strlen(buf)-1, 0, (struct sockaddr*)&cliAddr, cliLen);
            }
            if(FD_ISSET(sfd, &rdset)){
                memset(buf, 0, sizeof(buf));
                recvfrom(sfd, buf, sizeof(buf)-1, 0, (struct sockaddr*)&cliAddr, &cliLen);
                printf("buf = %s\n", buf);
            }
        }
    }

    return 0;
}

