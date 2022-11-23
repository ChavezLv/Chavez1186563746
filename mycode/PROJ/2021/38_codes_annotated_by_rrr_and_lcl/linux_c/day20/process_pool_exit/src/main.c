#include "process_pool.h"

int exitPipe[2];//全局变量 + 只有主进程看得见，是自己写来提醒自己的管道
void sigFunc(int signum)
{
    printf("sig is comming\n");
    write(exitPipe[1], &signum, 4);
}
int main(int argc, char** argv)
{
    //ip,port, processNum;
        
    int processNum = atoi(argv[3]);

    pProcess_data_t pData = (pProcess_data_t)calloc(processNum, sizeof(process_data_t));

    //1.循环创建子进程
    makeChild(processNum, pData);
    pipe(exitPipe);
    signal(SIGUSR1, sigFunc);

    //2.创建tcp监听套接字
    int sfd = 0;
    tcpInit(argv[1], argv[2], &sfd);

    //3.创建epoll
    int epfd = epoll_create(1);
    epollAddFd(sfd, epfd);
    epollAddFd(exitPipe[0], epfd);

    for(int i = 0; i < processNum; ++i){
        epollAddFd(pData[i].pipefd, epfd);
    }

    struct epoll_event evs[2];

    int newFd = 0;
    int readyNum = 0;

    while(1){
        readyNum = epoll_wait(epfd, evs, 2, -1);
        //此处如果放一个errorcheck就会发现epoll_wait是一个受到干扰的函数//epoll_wait是会收到signal干扰的，所以即使要用ERROR_CHECK
        //也不能再加上return了，不然函数会在此处就收到干扰-->所有的有【阻塞】的函数（比如一些I/O函数）也会有这个问题
        for(int i = 0; i < readyNum; ++i){
            //如果是sfd就绪，父进程就接收这次连接，返回的newfd交给一个空闲的子进程
            if(evs[i].data.fd == sfd){
                newFd = accept(sfd, NULL, NULL);
                for(int j = 0; j < processNum; ++j){
                    if(0 == pData[j].flag){
                        sendFd(pData[j].pipefd, newFd, 0);
                        //把任务交给非忙碌的子进程，并且把它的标志位置为1，表示忙碌
                        pData[j].flag = 1;
                        printf("child %d is busy\n", pData[j].pid);
                        break;
                    }
                }
                close(newFd);
            }
            if(evs[i].data.fd == exitPipe[0]){
                //退出方式1，比较暴力
                //一旦从exitPipe读端收到信号
                //依次kill所有子进程
                /* for(int j = 0; j < processNum; ++j){ */
                /*     kill(pData[j].pid, SIGUSR1); */
                /* } */

                //退出方式2，比较温和，让子进程自己主动退出
                for(int j = 0; j < processNum; ++j){
                    //把退出标记置为1，表示退出
                    //而子进程在每一个结尾检查一次退出标志，是否退出
                    sendFd(pData[j].pipefd, 0, 1);
                }
                for(int j = 0; j < processNum; ++j){
                    int status = 0;
                    wait(&status);
                    if(WIFEXITED(status)){
                        printf("exit num = %d\n", WEXITSTATUS(status));
                    }
                }
                printf("main exit\n");
                exit(0);
            }
            else{
                for(int j = 0; j < processNum; ++j){
                    if(pData[j].pipefd == evs[i].data.fd){

                        //从管道中读数据，把数据读出来
                        char buf[4] = {0};
                        read(pData[j].pipefd, buf, sizeof(buf));
                        //把子进程的状态置为0，表示子进程忙完了
                        pData[j].flag = 0;
                        printf("child %d is not busy\n", pData[j].pid);

                        break;
                    }
                }
            }
        }
    }


    return 0;
}

