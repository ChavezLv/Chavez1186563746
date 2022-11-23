#include <func.h>

int main(int argc,char*argv[]){
    ARGS_CHECK(argc, 3);


    int sfd = socket(AF_INET,SOCK_STREAM,0);
    ERROR_CHECK(sfd, -1, "socket");

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);

    struct sockaddr_in cAdrr;
    memset(&cAdrr, 0 , sizeof(cAdrr));
    socklen_t addrlen = sizeof(cAdrr);
    int ret = connect(sfd, (struct sockaddr*)& addr, addrlen);
    ERROR_CHECK(ret, -1, "connect");
    

    fd_set fdset;
    FD_ZERO(&fdset);

    char buf[1024] = {0};
    while(1){
        FD_SET(sfd,&fdset);
        FD_SET(STDIN_FILENO,&fdset); 
        int retSelect = select(10, &fdset, NULL, NULL,NULL);
        for(int i = 0; i < retSelect; ++i){
            if(FD_ISSET(STDIN_FILENO, &fdset)){
                memset(buf, 0, sizeof(buf));
                read(STDIN_FILENO, buf, sizeof(buf));
                send(sfd, buf, sizeof(buf)-1, 0);
            }
            if(FD_ISSET(sfd, &fdset)){
                memset(buf, 0,sizeof(buf));
                ret = recv(sfd, buf, sizeof(buf) - 1, 0);
                if(0 == ret){
                    printf("bye bye!");
                    break;    
                }
                printf("recv:%s",buf);
                fflush(stdout);
            }
        }
    }
    close(sfd);
    close(sfd);
    return 0;
}
