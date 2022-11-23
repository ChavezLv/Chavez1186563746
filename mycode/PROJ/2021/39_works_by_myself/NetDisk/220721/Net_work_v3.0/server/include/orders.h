#ifndef __ORDERS_H__
#define __ORDERS_H__
#include"../include/work_Queue.h"
#include"../include/database.h"
#define USERMAX 100
#define SALT_LEN 15
#define RANDOM_STR_LEN 8
#define PATH_LEN 512
#define PATH_NUM 20
#define DIR_NAME_LEN 20
#define COMMAN_LEN 512
enum orderNum{//命令类型，可扩充
    CD = 1,
    LS = 2,
    PWD = 3,
    PUTS = 4,
    GETS = 5,
    RM = 6,
    MKDIR = 7,
    RMDIR = 8,
    TREE = 9,
    ERROR = 10,
    SENDFILE = 11,
    RECVFILE = 12,
    LOAD_USERNAME = 13,
    LOAD_CRYPT = 14,
    REGISTER_USERNAME = 15,
    REGISTER_CRYPT = 16,
    EXIT = 17,
    NEWFD = 18
};

typedef struct Order_s{//客户端发送给服务端的命令
    int orderNum;//命令类型
    char orderTarget[ARGSLEN];//存放命令所需的参数
}Order_t, *pOrder_t;

typedef struct reOrder_s{
    int retNum;//存放需发送的标志
    char retInfo[1004];//存放发给客户端的信息，如存放错误信息，ls存放要显示的内容，要求客户端接到能直接打印。
}reOrder_t, *pReOrder_t;

int recv_Order(int clientFd, int *Fd, pPool_t pPoolOrder);
int read_Order(pWork_t pWork);//服务端由order中orderNum调用其他接口
int read_Addr(char *addr, int userId);
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


