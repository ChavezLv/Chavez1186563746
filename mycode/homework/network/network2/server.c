#include<func.h>
int main(void){
    int listenfd=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in addrs;
    memset(&addrs,0,sizeof(addrs));
    addrs.sin_addr.s_addr=inet_addr("192.168.3.119");//转为网络字节序
    addrs.sin_port=htons(8888);
    addrs.sin_family=AF_INET;
    bind(listenfd,&addrs,sizeof(addrs));
    int ret=listen(listenfd,10);
    while(1){
        printf("1\n");
        int peerfd=accept(listenfd,NULL,NULL);
        char buff[100]={0};
        printf("2\n");
        recv(peerfd,buff,100,0);
        printf("ser_recv:%s\n",buff);
        char buffs[100]="helloworld";
        printf("3\n");
        send(peerfd,buffs,strlen(buff),0);
        printf("4\n");
    }
        close(peerfd);
}
