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

    socklen_t serLen = sizeof(serAddr);
    char buf[64] = {0};
    sendto(sfd, "helloserver", 11, 0, (struct sockaddr*)&serAddr, serLen);

    recvfrom(sfd, buf, sizeof(buf)-1, 0, (struct sockaddr*)&serAddr, &serLen);
    printf("buf = %s\n", buf);

    close(sfd);
    return 0;
}

