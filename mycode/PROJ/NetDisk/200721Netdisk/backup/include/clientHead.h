#include<arpa/inet.h>
#include<dirent.h>
#include<errno.h>
#include<fcntl.h>
#include<grp.h>
#include<netdb.h>
#include<netinet/in.h>
#include<pthread.h>
#include<pwd.h>
#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/epoll.h>
#include<sys/ipc.h>
#include<sys/mman.h>
#include<sys/msg.h>
#include<sys/select.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<sys/socket.h>
#include<sys/stat.h>
#include<sys/time.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<time.h>
#include<unistd.h>

#define N 10
#define ERROR_CHECK(tar,flag,war){\
    if(tar == flag){\
        perror(war);\
        return -1;\
    }\
}
#define ARGS_CHECK(argc,num){\
    if(argc != num){\
        fprintf(stderr,"ARGS ERROR!\n");\
        return -1;\
    }\
}
#define PTHREAD_ERROR_CHECK(ret, funcName){\
    do{\
        if(ret != 0){\
            printf("%s : %s\n", funcName, strerror(ret));\
        }\
    }while(0);\
}
typedef struct child_Status{
    pid_t childPid;
    int Status;
    int childFd;
}child_Status_t, *pChild_Status_t;
typedef struct file_trans{
    int len;
    char buf[1020];
}File_t, *pFile_t;

int makeChild(int childNum, pChild_Status_t child);
int sendFd(int childFd, int *clientFd);
int recvFd(int childFd, int *clientFd);
int childWork(int childFd);
int tcp_init(char *ip, char *port);
int trans_File(int clientFd);
int recv_file(int sfd);
