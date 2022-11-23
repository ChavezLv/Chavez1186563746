/* #include "ftp.h" */
#include "../include/ftp.h"
#include "../include/timeCircleQue.h"

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
	if(fd_server == -1)
	{
		ERR_EXIT("socket");
	}

    /* TimeQue_t timeQue; */
    Map_t map;
    memset(&map, 0, sizeof(Map_t));
    int timefd;
    uint64_t exp;
    ssize_t s;
    timeQueInit(&map, &timefd);

    int epollfd = epoll_create(1);  //创建epoll实例对象

    epollAddFd(epollfd, timefd);
    
    int Fd[100] = 
	while(1)
	{
		fd_client = accept(fd_server, NULL, NULL);

		if(fd_client == -1)
		{
			ERR_EXIT("accept");
		}
        printf("fd: %d\n", fd_client);

        map.timeQue.rear = 0;
        timeQueIns(&map, fd_client);
        printf("ser_map:%d\n",map.timeQue.node[2].fdArr[fd_client].fd);
        sleep(1);
        map.timeQue.rear = 2;
        timeQueOut(&map);
        printf("ser_map:%d\n",map.timeQue.node[2].fdArr[fd_client].fd);

		bzero(log_buf, 128);
		sprintf(log_buf, "%d connect\n",++conn_num );
		write(1, log_buf, strlen(log_buf));
		ps = (psession_t)calloc(1, sizeof(*ps));
		ps ->sess_sfd = fd_client ;
		/* sleep(2); */
        /* timeQueOut(&map); */
		pid = fork() ;
		if(pid == 0)
		{
			client_handle(ps);
			exit(1);
		}
		free(ps);
	
       }
    
