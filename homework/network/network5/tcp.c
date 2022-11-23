#include"tcp.h" 
int tcpInit(const char *ip,unsigned short port){
    int listenfd=socket(AF_INET,SOCK_STREAM,0);
    ERROR_CHECK(listenfd, -1, "socket");

	int on = 1;
    int ret=setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
    ERROR_CHECK(ret, -1, "socket");
    ret=setsockopt(listenfd,SOL_SOCKET,SO_REUSEPORT,&on,sizeof(on));
    ERROR_CHECK(ret, -1, "socket");
    struct sockaddr_in addr;
    addr.sin_addr.s_addr=inet_addr(ip);
    addr.sin_family=AF_INET;
    addr.sin_port=htons(port);
    
    ret=bind(listenfd,(struct sockaddr*)&addr,sizeof(addr));
    ERROR_CHECK(ret,-1,"bind");
    ret= listen(listenfd,10);
    return listenfd;
}
//读取fd对应的内核缓冲区,根据返回值判断连接是否断开;设置MSG_PEEK标志位是只进行拷贝,
//不移除缓冲区的数据
int isConnectionClosed(int fd){
    int nret=0;
    char buff[20]={0};
    do{
        nret=recv(fd,buff,sizeof(buff),MSG_PEEK);
    }while(nret==-1&&errno==EINTR);
    return nret==0;
}

void epollEventLoop(int epfd,int listenfd){
    //定义一个结构体数组存放就绪的文件描述符
    struct epoll_event *evtList=(struct epoll_event*)calloc(MAXCONN,sizeof(struct epoll_event));
    while(1){
        int nready=epoll_wait(epfd,evtList,MAXCONN,-1);
        printf("ready:%d\n",nready);
        if(nready == -1 && errno == EINTR) {
			continue;
		} else if(nready == 0) {
			printf("epoll_wait timeout\n");
		} else if(nready == -1) {
			perror("epoll_wait");
//perror和strerror都是根据errno的值打印错误信息的。
//perror是将errno对应的错误消息的字符串打印到标准错误输出上，
//即stderr或2上，若你的程序将标准错误输出重定向到/dev/null，那就看不到了，
//就不能用perror了。而 strerror的作用只是将errno对应的错误消息字符串返回.
//你可以自己决定咋样处理字符串，比如可以存到日志，也可以直接printf出来。
		} else {

			for(int i = 0; i < nready; ++i) 
			{	//evtList数组中的元素一定是已经就绪的fd
				int fd = evtList[i].data.fd;
				if(fd == listenfd) {
                //处理新链接
					epollHandleNewConnection(epfd, listenfd);
				} else {
				//已经建立好的连接上是否有读事件发生
					epollHandleMessage(epfd, fd);
				}
			}
		}
    }
}


void epollHandleMessageConnection(int epfd,int listenfd){
    int peerfd=accept(listenfd,NULL,NULL);
    if(peerfd==-1){
        perror("accept");
        return;
    }
    epollAddReadEvent(epfd,peerfd);
}


void epollHandleMessage(int epfd,int fd){
    char buff[100]={0};
    int ret=recv(fd,buff,sizeof(buff),0);
    if(ret==0){
        printf("conn %d has closed",fd);
        epollDelEvent(epfd,fd);
        close(fd);
    }else if(ret==-1){
        perror("recv");
    }else {//成功读取,执行回显操作;
        send(fd,buff,strlen(buff),0);
    }
}


int epollAddReadEvent(int epfd,int fd){
    int ret=0;
    struct epoll_event evt;
    memset(&evt,0,sizeof(evt));
    evt.events=EPOLLIN; //默认是水平触发
    evt.data.fd=fd;
    ret=epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&evt);//添加到红黑树
    ERROR_CHECK(ret,-1,"epoll_ctl");
    return ret;
}

int epollDelEvent(int epfd, int fd)
{
	int ret = 0;
	struct epoll_event evt;
	memset(&evt, 0, sizeof(evt));
	evt.data.fd = fd;
	ret = epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &evt);
	ERROR_CHECK(ret, -1, "epoll_ctl");
	return ret;
}
