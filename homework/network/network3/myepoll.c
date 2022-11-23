#include<func.h>
#define MAXFD 1024
#define N 10
int main(){
    int listenfd=socket(AF_INET,SOCK_STREAM,0);
    int on=1;
    int ret=setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
    ret=setsockopt(listenfd,SOL_SOCKET,SO_REUSEPORT,&on,sizeof(on));
    struct sockaddr_in serveraddr;
    memset(&serveraddr,0,sizeof(serveraddr));
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_port=htons(8888);
    serveraddr.sin_addr.s_addr=inet_addr("192.168.3.119");
    ret=bind(listenfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
    ret =listen(listenfd,10);
    int epfd=epoll_create1(0);
    struct epoll_event evt;
    memset(&evt,0,sizeof(evt));
    evt.events=EPOLLIN;
    evt.data.fd=listenfd;
    ret=epoll_ctl(epfd,EPOLL_CTL_ADD,listenfd,&evt);
    struct epoll_event *evtList=(struct epoll_event*)calloc(MAXFD,sizeof(struct epoll_event));
    int count=0;
    int *tempfd;
    tempfd=(int *)calloc(N,sizeof(int));
    while(1){
        printf("the number of current connect:%d\n",count);
        int ready=epoll_wait(epfd,evtList,MAXFD,5000);
        printf("ready:%d\n",ready);
        if(ready==-1&&errno==EINTR)
            continue;
        else if(ready==0){
            printf("ready:%d\n",ready);
        }
        else if(ready==-1){
            perror("epoll_wait");
        }else{
            for(int i=0;i<ready;++i){
                int fd=evtList[i].data.fd;
                // printf("evtList[%i]:%d\n",i,fd);
                if(fd==listenfd){
                    int peerfd=accept(listenfd,NULL,NULL);
                    for(int i=0;i<N;i++){
                        if(tempfd[i]==0)
                        {   tempfd[i]=peerfd;
                            printf("peerfd:tempfd[%d]=%d\n",i,tempfd[i]);
                            break;
                        }
                    }
                    struct epoll_event evt;
                    memset(&evt,0,sizeof(evt));
                    //evt.events=EPOLLIN|EPOLLET;//默认是水平触发
                    evt.events=EPOLLIN;
                    evt.data.fd=peerfd;
                    ret=epoll_ctl(epfd,EPOLL_CTL_ADD,peerfd,&evt);
                    count++;
                }else{//peerfd事件的处理
                    char buff[100]={0};
                    memset(buff,'\0',sizeof(buff));
                    int ret=recv(fd,buff,sizeof(buff),0);//因为设置的是水平触发,所以当客户端以sizeof发送buff[1024]过来时,这边需要触发11次才能把客户端数据读取完!
                    printf("recv:%s\n",buff);
                    if(ret==0){//另一端关闭返回值0
                        printf("conn %d has closed\n",fd);
                        struct epoll_event evt;
                        evt.events=EPOLLIN;
                        evt.data.fd=fd;
                        ret=epoll_ctl(epfd,EPOLL_CTL_DEL,fd,&evt);
                        close(fd);
                        for(int i=0;i<N;i++){
                            if(tempfd[i]==fd)
                                tempfd[i]=0;
                        }
                        count--;
                    }else if(-1==ret&&errno==EINTR){
                        continue;
                    }else if(-1==ret){
                        perror("recv");
                    }else{//回显操作,每条消息都返回给客户端;
                        char msg[200] = {0};
                        for(int i=0;i<N;i++){
                            if(tempfd[i]!=0){
                                memset(msg,'\0',sizeof(msg));
                                sprintf(msg,"client[%d]:%s",fd,buff);
                                send(tempfd[i],msg,strlen(msg),0);
                                puts(msg);
                                printf("tempfd[%i]=%d\n",i,tempfd[i]);
                            }
                        }
                    }
                }
            }
        }
    }
    close(listenfd);
}

