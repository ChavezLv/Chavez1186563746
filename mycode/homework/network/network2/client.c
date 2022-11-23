#include<func.h>
//#include<sys/socket.h>
int main(void){
    printf("initial\n");
    int clientfd=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in addrs;
    memset(&addrs,0,sizeof(addrs));
    addrs.sin_addr.s_addr=inet_addr("192.168.3.119");//转为网络字节序
    addrs.sin_port=htons(8888);
    addrs.sin_family=AF_INET;
    printf("00\n");
    connect(clientfd,&addrs,sizeof(addrs));
    printf("0\n");
//  connect();
//  int ret=listen(listenfd,10);
//      int peerfd=accept(listenfd,NULL,NULL);
        char msg[100]="helloworld";
        printf("1");
        send(clientfd,msg,strlen(msg),0);
        char buffc[100]={0};
        printf("2");
        recv(clientfd,buffc,100,0);
        printf("client receive:%s\n",buffc);
        printf("3");
        close(clientfd);
    }

