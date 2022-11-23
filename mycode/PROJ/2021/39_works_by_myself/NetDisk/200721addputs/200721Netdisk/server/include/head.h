#ifndef __HEAD_H__
#define __HEAD_H__
#define _GNU_SOURCE
#include<arpa/inet.h>
#include <crypt.h>
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
#define ERROR_CHECK(ret,flag,warName){\
    if(ret == flag){\
        perror(warName);\
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
#define N 10


#endif
