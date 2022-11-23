#include <head.h>

int recvCycle(int sockFd, void *buf, int totalLen);

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

    char buf[1000] = {0};

    int dataLen = 0;
    recv(sfd, &dataLen, 4, 0);
    recv(sfd, buf, dataLen, 0);

    int fd = open(buf, O_RDWR|O_CREAT, 0666);

    off_t fileSize = 0;
    off_t recvLen = 0;
    
    recv(sfd, &dataLen, 4, 0);
    recv(sfd, &fileSize, dataLen, 0);
    printf("fileSize = %ld\n", fileSize);

    float rate = 0;

    

    /* while(1){ */
    /*     memset(buf, 0, sizeof(buf)); */
    /*     ret = recv(sfd, &dataLen, 4, 0); */

    /*     if(0 == dataLen){ */
    /*         break; */
    /*     } */
        
    /*     /1* ret = recv(sfd, buf, dataLen, MSG_WAITALL); *1/ */
    /*     ret = recvCycle(sfd, buf, dataLen); */
    /*     //计算此时总接收的字节数 */
    /*     recvLen += ret; */

    /*     rate = (float)recvLen / fileSize *100; */
    /*     printf("rate = %5.2f %%\r", rate); */
    /*     fflush(stdout); */
    /*     /1* if(1000 != ret){ *1/ */
    /*     /1*     printf("ret = %d\n", ret); *1/ */
    /*     /1* } *1/ */

    /*     write(fd, buf, ret); */
    /* } */
    printf("\n");
    close(sfd);
    return 0;
}

