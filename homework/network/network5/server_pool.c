#include"tcp.h"
#include"process_pool.h"
int main(int argc,char *argv[]){
    ARGS_CHECK(argc,4);
    //1,创建线程池
    int processNum=atoi(argv[3]);//将字符串转为整形数据
    pProcess_data_t pProcessData=(pProcess_data_t)calloc(processNum,sizeof(process_data_t));//用来保存子进程的数组
    makeChild(processNum,pProcessData);
    
    //2,监听套接字初始化(创建socket,绑定IP,监听)
    int listenfd=tcpInit(argv[1],atoi(argv[2]));

    //3.创建epoll对象,内核创建红黑树和双向链表,返回一个epoll描述符;然后将listenfd添加到红黑树上进行监听
    int epfd=epoll_create1(0);
    epollAddReadEvent(epfd,listenfd);
    for(int i=0;i<processNum;++i){
        epollAddReadEvent(epfd,pProcessData[i].pipefd);//对池内的每个子进程进行监听;
    }
    //4,创建数组存放wait返回的就绪队列
    struct epoll_event* pevtList=(struct epoll_event*)calloc(MAXCONN,sizeof(struct epoll_event));
    
    //5,循环监听listenfd和pipefd以及以链接到子进程的peerfd
    int nready=0;
    while(1){
        nready=epoll_wait(epfd,pevtList,MAXCONN,-1);//返回就绪描述符到pevtList
        printf("nready:%d\n",nready);
        if(nready==-1&&errno==EINTR){
            continue;
        }else if(nready == 0) {
			printf("epoll_wait timeout\n");
		} else if(nready == -1) {
			perror("epoll_wait");
		} else {
            for(int i=0;i<nready;i++){
                int fd=pevtList[i].data.fd;
                if(fd==listenfd){
                    int peerfd=accept(listenfd,NULL,NULL);
                    ERROR_CHECK(peerfd, -1, "accept");
    //6,监听到有新的链接到来,将新链接(peerfd)分配给空闲子进程
                    for(int j=0;j<processNum;i++){
                        if(pProcessData[j].busy==CHILD_PROCESS_FREE)
                        {
                             sendFd(pProcessData[j].pipefd,peerfd);
                             pProcessData[j].busy=CHILD_PROCESS_BUSY;
                             break;
                        }
                    }
                    close(peerfd);//关闭父进程与客户端的链接,使得只有子进程链接客户端;方便连接的断开
                }else{
    //7,子进程向父进程发送消息(如完成标志等)
                    char buff[10]={0};
                    read(fd,buff,sizeof(buff));
                    for(int j=0;j<processNum;++j){
                        if(pProcessData[j].pipefd==fd){
                            pProcessData[j].busy=CHILD_PROCESS_FREE;
                            printf("child %d is free\n",pProcessData[j].pid);
                            break;
                        }
                    }
                }
        }

}}}
