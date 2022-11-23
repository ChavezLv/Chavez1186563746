#include<func.h>
int main(){
    int clientfd=socket(AF_INET,SOCK_STREAM,0);
    int on=1;
    setsockopt(clientfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
    setsockopt(clientfd,SOL_SOCKET,SO_REUSEPORT,&on,sizeof(on));
    struct sockaddr_in addrc;
    memset(&addrc,0,sizeof(addrc));
    addrc.sin_addr.s_addr=inet_addr("192.168.3.119");
    addrc.sin_family=AF_INET;
    addrc.sin_port=htons(8888);
    connect(clientfd,(struct sockaddr*)&addrc,sizeof(addrc));
    fd_set fdset;
    FD_ZERO(&fdset);
    int maxfd=clientfd;
    if(maxfd<STDIN_FILENO){
        maxfd=STDIN_FILENO;
    }
    while(1){
        //一定要注意每次循环fdset都会改变!
    FD_SET(clientfd,&fdset);
    FD_SET(STDIN_FILENO,&fdset);
        int ret=select(maxfd+1,&fdset,NULL,NULL,NULL);
        if(ret==-1&&EINTR==errno) continue;
        if(FD_ISSET(STDIN_FILENO,&fdset)){
            char buff[1024]={0};
            read(STDIN_FILENO,buff,sizeof(buff));
            write(clientfd,buff,strlen(buff));//发送数据一定要用strlen,用sizeof会发送大量空数据
        }
        if(FD_ISSET(clientfd,&fdset)){
            char buff[1024]={0};
            int ret=read(clientfd,buff,sizeof(buff));
            if(ret==0){
                printf("server has closed\n");
                close(clientfd);
            }
            if(ret>0)
            printf("From %s\n",buff);
        }
    }
}
