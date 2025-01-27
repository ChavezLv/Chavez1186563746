#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/mman.h>
#include <sys/select.h>
#include <sys/time.h>
#include <pthread.h>
#include <setjmp.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/uio.h>
#include <arpa/inet.h>
#include <sys/shm.h>
#include <sys/epoll.h>
#define ARGS_CHECK(argc, val) \
    if(argc != val) { \
        printf("error args\n"); \
        return -1; \
    } 


#define ERROR_CHECK(ret, retVal, funcName) { \
    if(ret == retVal) { \
        perror(funcName); \
        return -1; \
    } \
}

#define THREAD_ERROR_CHECK(ret, funcName) \
    do { \
        if (0 != ret) { \
            printf("%s : %s\n", funcName, strerror(ret)); \
        } \
    }while(0)


//父进程管理子进程的数据结构
typedef  struct {
    short flag;
    int pipefd;
    pid_t pid;
}process_data_t, *pProcess_data_t;

typedef struct{
    int len;
    char buf[1000];
}Train_t;

int makeChild(int processNum, pProcess_data_t pData);
int childFunc(int pipefd);
int tcpInit(char *ip, char *port, int *sockFd);
int epollAddFd(int fd, int epfd);
int sendFd(int pipeFd, int fd);
int recvFd(int pipeFd, int *fd);
int transFile(int clienFd);
