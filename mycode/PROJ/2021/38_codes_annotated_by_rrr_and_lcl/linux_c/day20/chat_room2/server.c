#include <head.h>
//设置客户端最大连接数
#define CNUM 20

typedef struct{
    int cliFd;
    char online;
}Client_t ,*pClient_t;

int main(int argc,char* argv[])
{
    int sfd;
    //监听套接字
    sfd = socket(AF_INET,SOCK_STREAM,0);
    printf("sfd=%d\n",sfd);

    int ret = 0;
    struct sockaddr_in addr;
    memset(&addr,0,sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atoi(argv[2]));

    //设置套接口选项，允许地址重用
    int reuse = 1;
    setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));

    ret = bind(sfd,(struct sockaddr*)&addr,sizeof(addr));
    ERROR_CHECK(ret,-1,"bind");

    ret = listen(sfd,10);
    ERROR_CHECK(ret,-1,"listen");

    int newFd = 0;

    Client_t client[CNUM];

    /* memset(client, 0, CNUM * sizeof(Client_t)); */
    memset(client, 0, sizeof(client));

    char buf[64]={0};
    //存放所有需要读的文件描述符
    fd_set rdset;
    fd_set needMonitorSet;
    FD_ZERO(&rdset);
    FD_ZERO(&needMonitorSet);
    int clinum=0;

    int nready=0;
    //服务端只做消息转发
    FD_SET(sfd,&needMonitorSet);
    while(1)
    {
        FD_ZERO(&rdset);
        memcpy(&rdset,&needMonitorSet,sizeof(needMonitorSet));
        nready=select(25,&rdset,NULL,NULL,NULL);
        ERROR_CHECK(nready, -1, "select");

        //如果有客户端连接，接收并将newFd存储到结构体中。
        if(FD_ISSET(sfd,&rdset)){
            newFd = accept(sfd, NULL, NULL); 
            FD_SET(newFd, &needMonitorSet);
            send(newFd, "welcome to join the chat room!", 30, 0);

            //把newFd存到客户端结构体当中,且最多能接受CNUM个客户端的连接
            for(int i = 0; i < CNUM; i++){
                if(client[i].online == 0){
                    client[i].cliFd = newFd;
                    //把客户端状态置为在线
                    client[i].online = 1;
                    //客户端数量++
                    clinum++;
                    break;
                }
            }
        }

        //客户端发消息,接收并转发给其它在线客户端
        else{
            memset(buf, 0, sizeof(buf));
            int i = 0;

            //循环查找是哪个客户端发过来的消息
            for(i = 0; i < CNUM; i++){
                if(FD_ISSET(client[i].cliFd, &rdset)){
                    ret = recv(client[i].cliFd, buf, sizeof(buf), 0);
                    if(0 == ret){
                        //recv返回值等于0，说明对端断开了
                        for(int j = 0; j < CNUM; j++){
                            if(i != j){
                                if(1 == client[j].online){
                                    char buf1[16] = {0};
                                    sprintf(buf1, "client %d exit", client[i].cliFd);
                                    send(client[j].cliFd, buf1, strlen(buf1), 0);
                                }
                            }
                        }

                        client[i].online = 0;
                        FD_CLR(client[i].cliFd,&needMonitorSet);
                        close(client[i].cliFd);
                    }
                    else{
                        //遍历的方式发送给其他客户端
                        for(int j = 0; j < CNUM; j++){
                            if(i != j){
                                if(1 == client[j].online){
                                    char buf2[64] = {0};
                                    sprintf(buf2, "%d: %s", client[i].cliFd, buf);
                                    send(client[j].cliFd, buf2, strlen(buf2), 0);
                                }
                            }
                        }
                    }
                    break;
                }
            }
        }
    }

    close(newFd);
    close(sfd);
    return 0;
}
