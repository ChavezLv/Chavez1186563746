 ///
 /// @file    main.c
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2022-10-07 11:07:35
 ///
 
#include "threadpool.h"
#include "tcp.h"

int exitPipe[2];

void sigFunc(int signum)
{
	printf("signum is coming\n");
	write(exitPipe[1], "a", 1);
}
 
int main(int argc, char *argv[])
{
	//ip,port,threadNum
	ARGS_CHECK(argc, 4);

	signal(SIGUSR1, sigFunc);

	pipe(exitPipe);
	pid_t ret = fork();
	if(ret > 0) {
		wait(NULL);//等待子进程退出
		exit(0);//父进程退出
	}

	//以下的行为都是子进程的
	//线程池在子进程中运行
	
	int threadNum = atoi(argv[3]);
	//创建线程池,并运行
	threadpool_t threadpool;
	threadpoolInit(&threadpool, threadNum);
	threadpoolStart(&threadpool);

	//创建监听套接字
	int listenfd = tcpInit(argv[1], atoi(argv[2]));

	//创建epoll的实例
	int epfd = epoll_create1(0);
	ERROR_CHECK(epfd, -1, "epoll_create1");
	epollAddReadEvent(epfd, listenfd);
	epollAddReadEvent(epfd, exitPipe[0]);

	struct epoll_event * pevtList = (struct epoll_event*)
		calloc(MAXCONN, sizeof(struct epoll_event));
	int nready = 0;

	//进入事件循环
	while(1) {
		nready = epoll_wait(epfd, pevtList, MAXCONN, 5000);
		if(nready == -1 && errno == EINTR) {
			continue;
		} else if(nready == -1) {
			perror("epoll_wait");
		} else if(nready == 0) {
			printf("epoll_wait timeout\n");
		} else {
			
			for(int i = 0; i < nready; ++i) {
				int fd = pevtList[i].data.fd;
				if(fd == listenfd) {
					int peerfd = accept(listenfd, NULL, NULL);
					if(peerfd == -1) {
						perror("accept");
						continue;
					}
					//构造任务节点，将其放入任务队列
					taskEnqueue(&threadpool.queue, peerfd);
					printf("peerfd has enter the queue\n");
				}

				if(fd == exitPipe[0]) {
					char ch = 0;
					read(exitPipe[0], &ch, 1);
					printf("threadpool begin exit\n");
					//线程池退出
					threadpoolStop(&threadpool);
					threadpoolDestroy(&threadpool);
					exit(0);
				}
			}
		}
	}
	close(listenfd);
	close(epfd);
}
