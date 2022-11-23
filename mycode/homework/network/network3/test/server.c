#include<func.h>
#define MAXFD 1024
int main(){
    int listenfd=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in addrs;
    memset(&addrs,0,sizeof(addrs));
    addrs.sin_addr.s_addr=inet_addr("192.168.3.119");
    addrs.sin_port=htons(8888);
    addrs.sin_family=AF_INET;
    bind(listenfd,(struct sockaddr*)&addrs,sizeof(addrs));
    listen(listenfd,10);
    fd_set fdset;
    FD_ZERO(&fdset);
    int nfds[MAXFD]={0};
    int maxfd=listenfd;
    int peerfd=1;
    while(1){
        //每次循环重设fdset;
        FD_ZERO(&fdset);
        FD_SET(listenfd,&fdset);
        FD_SET(STDIN_FILENO,&fdset);
        for(int i=0;i<maxfd;i++){
            if(nfds[i]!=0){
                FD_SET(nfds[i],&fdset);
            }
        }
        //监听listenfd,若有新链接,置入fdset
        int ready=select(maxfd+1,&fdset,NULL,NULL,NULL);
        if(ready<0&&errno==EINTR) continue;
        if(FD_ISSET(STDIN_FILENO,&fdset)){
            char buff[1024]={0};
            read(STDIN_FILENO,buff,sizeof(buff));
            //fflush(stdout);
            write(peerfd,buff,strlen(buff));
        }
        if(FD_ISSET(listenfd,&fdset)){
            peerfd=accept(listenfd,NULL,NULL);
            for(int i=0;i<MAXFD;i++){
                if(nfds[i]==0){
                      nfds[i]=peerfd;
                      break;
                    }
                }
            if(maxfd<peerfd) maxfd=peerfd;
            }
        //处理已连接的文件描述符
        //if(FD_ISSET(peerfd,&fdset)){//fdset应该包含多个peerfd
        for(int i=0;i<maxfd+1;i++){
            if(nfds[i]!=0){
                if(FD_ISSET(nfds[i],&fdset)){
                    char buff[1024]={0};
                    int ret=recv(nfds[i],buff,sizeof(buff),0);
                    printf("recv:\nfrom client[%d]:%s\n",nfds[i],buff);
                    if(ret==-1&&errno==EINTR)
                        continue;                        
                    else if (ret==0){//超时等待
                        printf("Because of timeout that %d has been closed!",nfds[i]);
                        close(nfds[i]);
                       // break;
                        }
                    else if(ret==-1)
                        perror("recv");
                    else//执行回显操作
                         send(nfds[i],buff,sizeof(buff),0);
    
}}}}}
