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

    char buf[64] = {0};

    recv(sfd, buf, sizeof(buf)-1, 0);
    printf("buf = %s\n", buf);

    send(sfd, "helloserver", 11, 0);

    close(sfd);
    return 0;
}

