#ifndef __HEAD_H__
#define __HEAD_H__

/* #define _GNU_SOURCE //用于splice的宏 */
#include<stdio.h>
#include<errno.h>
#include<string.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<unistd.h>
#include<dirent.h>
#include<pwd.h>
#include<grp.h>
#include<fcntl.h>  
#include<sys/types.h>
#include <time.h>
#include<sys/select.h>
#include<sys/mman.h>
#include<sys/wait.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<sys/msg.h>
#include<pthread.h>
#include<sys/time.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<sys/epoll.h>
#include<sys/sendfile.h>
#include <sys/ioctl.h> //ioctl
#include<syslog.h> //syslog
/* #define DEBUG */

#define ARGS_CHECK(argc,num) \
    do{\
        if(argc!=num){\
            printf("error argcs!\n");\
            return -1;\
        }\
    }while(0)

#define ERROR_CHECK(ret,num,msg) \
    do{\
        if(ret==num){\
            perror(msg);return -1;\
        }\
    }while(0)

    //{printf("error!\n");return -1;}}

#define THREAD_ERROR_CHECK(ret,funcName) \
    do{\
        if(0!=ret){\
            printf("%s:%s-%d\n",funcName,strerror(ret),__LINE__);\
        }\
    }while(0)

#define PRINT_CHECK(x)   do{\
    printf("%s = %d\n", #x, x);\
    }while(0)

#define RETURN_CHECK(x,y)   do{\
    printf("%s's %s = %d\n", #y, #x, x);\
    }while(0)

//循环创建子进程，记录子进程相关信息--结构体设计
typedef struct ProcessPool_s{
    int flag;//子进程状态
    pid_t pid;//
    int pipeFd;
}ProcessPool_t,*pProcessPool_t;

typedef struct DataTrain_s{
    /* size_t len; */
    int len;
    char buf[1024];
    /* char buf[1000]; */
}DataTrain_t,*pDataTrian_t;

int sendFd(int pipeFd, int fd);
int recvFd(int pipeFd, int *fd);
int processPoolInit(pProcessPool_t *ppProcessPool, int processPoolNum); 
int processPoolCreate(pProcessPool_t pProcessPool, int processPoolNum);
int epollAdd(int epfd,int fd);
int epollDel(int epfd, int fd);
int processPoolFunc(int sockPairFd);

//daemon
void Daemon();

//function of client
int recvDataTrain(int sfd, char *trainBuf, int dataTrainLen);
int mmapDataTrain(int sfd, char *pTransFile,int dataTrainLen);



#endif
