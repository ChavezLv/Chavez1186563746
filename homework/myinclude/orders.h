#ifndef __orders_H__
#define __orders_H__
#include"work_Queue.h"
#include"database.h"
#define USERMAX 100
#define SALT_LEN 15
#define RANDOM_STR_LEN 8
#define PATH_LEN 512
#define PATH_NUM 20
#define DIR_NAME_LEN 20
#define COMMAN_LEN 512
enum orderNum{
    CD=1,
    LS,PWD,PUTS,GETS,RM,MKDIR,RMDIR,TREE,ERROR,SENDFILE,RECVFILE
};//拓展命令

typedef  struct Order_s{
    int orderNum;//命令号
    char orderTarget[ARGSLEN];//命令所需参数
}Order_t,*pOrder_t;

typedef  struct reOrder_s{
    int retNum;
    char retInfo[1024];//回显给客户端的信息
}reOrder_t,*pReOrder_t;

int recvOrd(int clientFd,int *fd,pPool_t pPoolOrd);
int readOrd(pWork_t pWork);
int readAddr(char *addr,int userId);

int order_cd(pReOrder_t pReOrder, pWork_t pWork);
int order_ls(pReOrder_t pReOrder, pWork_t pWork);
int order_pwd(pReOrder_t pReOrder, pWork_t pWork);
int order_puts(pReOrder_t pReOrder, pWork_t pWork);
int order_rm(pReOrder_t pReOrder, pWork_t pWork);
int order_mv(pReOrder_t pReOrder, pWork_t pWork);
int order_rmdir(pReOrder_t pReOrder, pWork_t pWork);
int order_mkdir(pReOrder_t pReOrder, pWork_t pWork);
int order_tree(pReOrder_t pReOrder, pWork_t pWork);

int userName(pReOrder_t reOrder, pWork_t pWork);
int userCrypt(pReOrder_t reOrder, pWork_t pWork);
int handleRegisterRecvCrypt(pReOrder_t reOrder, pWork_t pWork);
int handleRegisterRecvUserName(pReOrder_t reOrder, pWork_t pWork);
char *genRandomString(void);
char *genCipherText(char *salt, char *password);

#endif
