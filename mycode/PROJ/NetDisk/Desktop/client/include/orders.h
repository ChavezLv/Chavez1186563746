#ifndef __ORDERS_H__
#define __ORDERS_H__
#include"../include/head.h"
#include"../include/work_Queue.h"
#define MAX 10
enum orderNum{
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

typedef struct Order_s{
    int orderNum;
    char orderTarget[ARGSLEN];
}Order_t, *pOrder_t;

typedef struct reOrder_s{
    int reOrderNum;
    char reInfo[ARGSLEN];
}reOrder_t, *pReOrder_t;

int beginSystem(int sfd);
int send_Order(int sfd);
int changeOrder(char *initOrder, int *orderNum);
int read_Order(int sfd);
#endif


