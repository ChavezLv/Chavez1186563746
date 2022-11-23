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
    
    ret = listen(sfd, 10);
    ERROR_CHECK(ret, -1, "listen");
    
    struct sockaddr_in clientAdrr;
    memset(&clientAdrr, 0 , sizeof(clientAdrr));
    socklen_t clientAdrrLen = sizeof(clientAdrr);
    /* int newFd = accept(sfd, NULL,NULL); */
    int newFd = accept(sfd, (struct sockaddr*)& clientAdrr,&clientAdrrLen);
    ERROR_CHECK(newFd, -1, "accept");

    fd_set fdset;
    FD_ZERO(&fdset);

    char buf[1024] = {0};
    int flag = 1;
    while(1){ 
        if(1 == flag){
            FD_SET(newFd,&fdset);
        }
        FD_SET(STDIN_FILENO,&fdset); 
        FD_SET(sfd,&fdset);
        int retSelect = select(10, &fdset, NULL, NULL,NULL);
        ERROR_CHECK(retSelect, -1, "select");
        for(int i = 0; i < retSelect; ++i){
            if(FD_ISSET(STDIN_FILENO, &fdset)){
                memset(buf, 0, sizeof(buf));
                read(STDIN_FILENO, buf, sizeof(buf));
                send(newFd, buf, sizeof(buf)-1, 0);
            }
            if(FD_ISSET(newFd, &fdset)){
                memset(buf, 0,sizeof(buf));
                ret = recv(newFd, buf, sizeof(buf) - 1, 0);
                if(0 == ret){
                    close(newFd);
                    FD_CLR(newFd,&fdset);//这时候出了循环因为newFd可能会被错误FD_SET
                    printf("bye bye!\n");
                    flag = 0;
                    continue;
                }
                printf("recv:%s",buf);
                fflush(stdout);
            }
            if(FD_ISSET(sfd,&fdset)){
                newFd = accept(sfd, NULL, NULL);
                ERROR_CHECK(newFd, -1, "accept");
                flag = 1; 
            }
        }
    }
    close(sfd);
    close(newFd);
    return 0;
}
