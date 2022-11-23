#define _GNU_SOURCE
#include <head.h>

int recvCycle(int sockFd, void *buf, int totalLen);

int main(int argc, char** argv)
{
    int sfd = socket(AF_INET, SOCK_STREAM, 0); 
    ERROR_CHECK(sfd, -1, "socket");

    struct sockaddr_in serAddr;
    memset(&serAddr, 0, sizeof(serAddr));
    serAddr.sin_family = AF_INET;   //
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

    struct timeval begin, end;

    gettimeofday(&begin, NULL);

    /* ftruncate(fd, fileSize); */

    /* char * pMap = (char*)mmap(0, fileSize, PROT_WRITE|PROT_READ, MAP_SHARED, fd, 0); */
    /* ERROR_CHECK(pMap, (char*)-1, "mmap"); */

    /* recv(sfd, pMap, fileSize, MSG_WAITALL); */

    /* munmap(pMap, fileSize); */

    
    int sfd1[2];
    pipe(sfd1);
    
    int recvLen1 = 0;
    while(recvLen1 < fileSize){
        int ret = splice(sfd, 0, sfd1[1], 0, 128, 0);
        ret = splice(sfd1[0], 0, fd, 0, ret, 0);
        recvLen1 += ret;
    }




    /* while(1){ */
    /*     memset(buf, 0, sizeof(buf)); */
    /*     ret = recv(sfd, &dataLen, 4, 0); */

    /*     if(0 == dataLen){ */
    /*         break; */
    /*     } */
        
        /* ret = recv(sfd, buf, dataLen, MSG_WAITALL); */
        /* ret = recvCycle(sfd, buf, dataLen); */
        //计算此时总接收的字节数
        /* recvLen += ret; */

        /* rate = (float)recvLen / fileSize *100; */
        /* printf("rate = %5.2f %%\r", rate); */
        /* fflush(stdout); */
        /* if(1000 != ret){ */
        /*     printf("ret = %d\n", ret); */
        /* } */

        /* write(fd, buf, ret); */
    /* } */
    /* printf("\n"); */
    /* gettimeofday(&end, NULL); */
    /* printf("cost time = %ld\n", (end.tv_sec - begin.tv_sec)*1000000 + (end.tv_usec - begin.tv_usec)); */
    close(sfd);
    return 0;
}

