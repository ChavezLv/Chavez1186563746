#include"orders.h"//所有命令接口
#include"tcp.h"
#include"threadpool.h"
#define MAXUSER 64 
int main (int argc,char* argv[]){
    threadPool_Init();//补入参数
    int threadNum=atoi(argv[1]);//启动服务端的时候定义进程池数目
    int listenfd=tcp_Init();//ip,port
    int ListFD[MAXUSER];//存放tcp连接的数组,参数为宏定义
    ListFD[listenfd]=listenfd;
    
    struct epoll_event evt=calloc();//空间分配
    int epfd=epoll_create1(0);
    epoll_Add(epfd,listenfd);
    int nready =0;
    while(1){
        nready=epoll_pwait();
        if(nready){//判断nready返回值

    }else{
        for(int i=0;i<nready;i++){
            if(evt[i].data.fd==listenfd){
                int peerfd=accept();//补充参数
                printf("msg\n");//打印反馈信息
                epoll_Add(epfd,peerfd);
                //此处需要创建数组存放已登录用户
            }
            else{
                recv_order();//所有命令接口
            }
    }
    
}}
