#include "../include/ftp.h"
#include "../include/timeCircleQue.h"
/* #define MAXNUM 5 */
void sig_handle(int signum)
{
    printf("sig is comming\n");
}

int main(int argc, char* argv[])// conf_file path
{
	int pid ;
	int conn_num = 0 ;
	char log_buf[128];
	char ip[16] = "" ;
	char port[10]= "";
	psession_t ps ;

	signal(10, sig_handle);
	ftp_init_from_file(argv[1], ip, port );

	socket_t fd_server = socket_server(ip, port);
	socket_t fd_client ; 

    /* TimeQue_t timeQue; */
    Map_t map;
    int timefd;
    uint64_t exp;
    ssize_t s;
    timeQueInit(&map, &timefd);
    
    int epollfd = epoll_create(1);  //创建epoll实例对象

    struct epoll_event ev;
    /* struct epoll_event events[MAXNUM]; */
    struct epoll_event events[3];
    ev.data.fd = timefd;
    ev.events = EPOLLIN | EPOLLET;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, timefd, &ev); //添加到epoll事件集合

    /* struct epoll_event ev; */
    /* struct epoll_event events[MAXNUM]; */
    ev.data.fd = fd_server;
    ev.events = EPOLLIN | EPOLLET;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd_server, &ev); //添加到epoll事件集合
    
	if(fd_server == -1)
	{
		ERR_EXIT("socket");
	}
	while(1)
	{
        int num = epoll_wait(epollfd, events, 3, -1);
        for (int i = 0; i < num; i++) 
        {
            /* if (events[i].events & EPOLLIN) */ 
            /* { */
                //....处理其他事件
                if (events[i].data.fd == timefd) 
                {
                    s = read(events[i].data.fd, &exp, sizeof(uint64_t)); //需要读出uint64_t大小, 不然会发生错误
                    timeQueRearChange(&map);
                    timeQueOut(&map);
                    printf("time out\n");
                }
                if(events[i].data.fd == fd_server){
                    printf("ser:%d\n", fd_server);
                    fd_client = accept(fd_server, NULL, NULL);
                    if(fd_client == -1)
                    {
                        ERR_EXIT("accept");
                    }
                    printf("ser:%d\n", fd_client);
                    timeQueIns(&map, fd_client);
                    bzero(log_buf, 128);
                    sprintf(log_buf, "%d connect\n",++conn_num );
                    write(1, log_buf, strlen(log_buf));

                    ps = (psession_t)calloc(1, sizeof(*ps));
                    ps ->sess_sfd = fd_client ;
                    printf("ser:%d\n", fd_client);
                    pid = fork() ;
                    if(pid == 0)
                    {
                        client_handle(ps);
                        exit(1);
                    }
                    free(ps);
                }
            /* } */
        }

	}

}
