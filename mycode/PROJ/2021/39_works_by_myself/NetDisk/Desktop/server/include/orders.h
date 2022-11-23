#ifndef __ORDERS_H__
#define __ORDERS_H__
#include"../include/work_Queue.h"
#include"../include/database.h"
#include"../include/user.h"
#define USERMAX 100
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
    LOAD = 11,
    REGISTER = 12,
    EXIT = 13
};

typedef struct Order_s{//客户端发送给服务端的命令
    int orderNum;//命令类型
    char orderTarget[ARGSLEN_SMALL];//存放命令所需的参数
}Order_t, *pOrder_t;

typedef struct reOrder_s{
    int retNum;//存放需发送的标志
    char retInfo[1024];//存放发给客户端的信息，如存放错误信息，ls存放要显示的内容，要求客户端接到能直接打印。
}reOrder_t, *pReOrder_t;
int recv_Order(int clientFd, int *Fd, pPool_t pPoolOrder);
int read_Order(void* order, int clientFd, int *Fd);//服务端由order中orderNum调用其他接口
int read_Addr(char *addr);
//输入一个路径，能返回路径所指的目录id，如/user/include，要返回include的目录id
//需要查询目录表
int order_cd(pReOrder_t pReOrder, char *orderTarget, int userId);
//输入reOrder指针，命令参数(即cd ..的..)，和发命令的userId
//返回正确失败，修改reOrder的retInfo，retInfo存放错误或成功信息。
//接口工作内容为根据命令参数，将用户表中的pwd修改为命令参数所指目录。如cd /user ，要讲pwd改为user的目录id
//需要调用read_Addr，需要更新用户表。
int order_ls(pReOrder_t pReOrder, char *orderTarget, int userId);
//需要输入reOrder指针，命令参数(即ls ..的..)，和发命令的userId
//返回正确失败，修改reOrder的retInfo，retInfo存放ls 的结果，应为可以直接printf的字符串(需要控制格式以达到好的显示效果)。
//需要查询目录表和用户文件表
int order_pwd(pReOrder_t pReOrder, int userId);
//需要输入reOrder指针，发命令的userId
//返回正确失败，修改reOrder的retInfo，retInfo存放pwd的结果或错误信息
//需要查询用户信息表和目录表
int order_puts(pReOrder_t pReOrder, char *orderTarget, int userId);
//需要查询文件信息表，用户文件表，**更改文件信息表和用户文件表**
int order_gets(pReOrder_t pReOrder, char *orderTarget, int userId);
//需要查询文件信息表，用户文件表
int order_rm(pReOrder_t pReOrder, char *orderTarget, int userId);
//需要查询和**修改目录表**
int order_rmdir(pReOrder_t pReOrder, char *orderTarget, int userId);
//需要查询和**修改目录表**
int order_mkdir(pReOrder_t pReOrder, char *orderTarget, int userId);
//需要查询和**修改目录表**                                                                                                      
int order_tree(pReOrder_t pReOrder, char *orderTarget, int userId);
//需要查询目录表和用户文件表
#endif


