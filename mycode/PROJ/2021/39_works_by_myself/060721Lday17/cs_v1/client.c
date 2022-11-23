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
    

    char buf[1024] = {0};
    ret = send(sfd, "hello", 5, 0); 
    ERROR_CHECK(ret, -1, "send");
    ret = recv(sfd, buf, sizeof(buf), 0);  

    close(sfd);
    close(sfd);
    return 0;
}
