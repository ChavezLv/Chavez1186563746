#include <func.h>

int main(int argc,char*argv[]){
    ARGS_CHECK(argc, 3);


    int sfd = socket(AF_INET,SOCK_STREAM,0);
    ERROR_CHECK(sfd, -1, "socket");

    
    struct sockaddr_in serAddr;
    memset(&serAddr, 0, sizeof(serAddr));
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(atoi(argv[2]));
    serAddr.sin_addr.s_addr = inet_addr(argv[1]);

    int ret = bind(sfd, (struct sockaddr*)& serAddr, sizeof(struct sockaddr));
    ERROR_CHECK(ret, -1 ,"bind");
    
    int rcvbuf = 0;
    socklen_t len = sizeof(rcvbuf);
    getsockopt(sfd, SOL_SOCKET, SO_RCVBUF, &rcvbuf, &len);
    printf("%d\n",rcvbuf);

    ret = listen(sfd, 10);
    ERROR_CHECK(ret, -1, "listen");
    
    struct sockaddr_in cAdrr;
    memset(&cAdrr, 0 , sizeof(cAdrr));
    socklen_t serAddrlen = sizeof(cAdrr);
    /* int newFd = accept(sfd, NULL,NULL); */
    int newFd = accept(sfd, (struct sockaddr*)& serAddr,&serAddrlen);
    ERROR_CHECK(newFd, -1, "accept");

    char buf[5] = {0};
    while(1){
        sleep(1);
        ret = recv(newFd, buf, sizeof(buf), 0);  
        printf("buf= %s\n",buf);
    }
    /* ret = send(newFd, "hello", 5, 0); */ 
    /* ERROR_CHECK(ret, -1, "send"); */

    close(sfd);
    close(newFd);
    return 0;
}
