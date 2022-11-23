#include <func.h>

int main(int argc,char*argv[]){
    ARGS_CHECK(argc, 3);


    int sfd = socket(AF_INET,SOCK_DGRAM,0);
    ERROR_CHECK(sfd, -1, "socket");

    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);

    
    char buf[1024] = {0};

    int ret = sendto(sfd, "helloserver", 12, 0, (struct sockaddr*)&addr, len);
    ERROR_CHECK(ret, -1, "sendto");
    
    ret = recvfrom(sfd, buf, sizeof(buf), 0, NULL, NULL);
    printf("buf:%s\n",buf);
    
    close(sfd);
    close(sfd);
    return 0;
}
