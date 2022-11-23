#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<pthread.h>
#include<unistd.h>
#include<fcntl.h>
#include<signal.h>
#include<string.h>
#include<sys/types.h>
#include<sys/fcntl.h>
#include<sys/errno.h>
#include<sys/stat.h>
#include<dirent.h>
#include<pwd.h>
#include<stdbool.h>
#include<head.h>
#include<mysql/mysql.h>
typedef __sighandler_t sighandler_t;
#ifndef SIZE

#define SIZE(a) (sizeof(a)/sizeof(a[0]))

#define ARGS_CHECK(argc,n) {     \
    if (argc!=n){                \
        fprintf(stderr,"args error:expected %d arguments.\n",n);     \
        exit(1);                        \
    }                           \
}                   \
#endif

#ifndef ERROR_CHECK

#define ERROR_CHECK(retval,val,msg) {     \
    if (retval==val){                \
        perror(msg);                \
        exit(1);                        \
    }                           \
}                              \

#endif

#ifndef THREAD_ERROR_CHECK

#define THREAD_ERROR_CHECK(err,msg) do{\
    if(err){\
        fprintf(stderr,"%s:%\n",msg,strerro(err));\
    }\
}while(0)\      //防止用于if条件语句时导致else失效

#endif


