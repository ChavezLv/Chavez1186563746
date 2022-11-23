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
    ret = bind(sfd, (struct sockaddr*)&serAddr, sizeof(serAddr));
    ERROR_CHECK(ret, -1, "bind");

    struct sockaddr_in cliAddr;
    memset(&cliAddr, 0, sizeof(cliAddr));

    socklen_t cliLen = sizeof(cliAddr);
    char buf[64] = {0};
    //必须先收数据，因为udp是非面向连接的，所以需要先接收对方数据包，把对方的ip和port存储在cliAddr中
    recvfrom(sfd, buf, sizeof(buf)-1, 0, (struct sockaddr*)&cliAddr, &cliLen);
    printf("buf = %s\n", buf);

    sendto(sfd, "helloclient", 11, 0, (struct sockaddr*)&cliAddr, cliLen);

    return 0;
}

