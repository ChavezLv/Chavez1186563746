#include <func.h>

int main(int argc,char*argv[]){
    ARGS_CHECK(argc, 3);

    int sfd = socket(AF_INET,SOCK_DGRAM,0);
    ERROR_CHECK(sfd, -1, "socket");
    
    struct sockaddr_in serAddr;
    memset(&serAddr, 0, sizeof(serAddr));
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(atoi(argv[2]));
    serAddr.sin_addr.s_addr = inet_addr(argv[1]);

    int ret = bind(sfd, (struct sockaddr*)& serAddr, sizeof(struct sockaddr));
    ERROR_CHECK(ret, -1 ,"bind");
    
    
    struct sockaddr_in clientAdrr;
    memset(&clientAdrr, 0 , sizeof(clientAdrr));
    socklen_t clientAdrrLen = sizeof(clientAdrr);
    /* socklen_t len = sizeof(clientAdrr); */

    char buf[1024] = { 0 };
    ret = recvfrom(sfd, buf, sizeof(buf), 0,(struct sockaddr*) &clientAdrr, &clientAdrrLen);
    ERROR_CHECK(ret, -1, "recvfrom");
    printf("buf = %s\n",buf);

    ret = sendto(sfd, "helloclient", 12, 0, (const struct sockaddr*)&clientAdrr, clientAdrrLen);
    close(sfd);
    /* close(newFd); */
    return 0;
}
