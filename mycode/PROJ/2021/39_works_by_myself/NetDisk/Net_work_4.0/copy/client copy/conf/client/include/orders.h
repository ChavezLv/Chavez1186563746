#ifndef __ORDERS_H__
#define __ORDERS_H__
#include"../include/head.h"
#include"../include/work_Queue.h"
#include<crypt.h>
#include"../include/md5.h"
#define MAX 10
#define SALT_LEN 15
#define RANDOM_STR_LEN 8
#define ARGSLEN 1000 
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
    SENDFILE = 11,
    RECVFILE = 12,
    LOAD_USERNAME = 13,
    LOAD_CRYPT = 14,
    REGISTER_USERNAME = 15,
    REGISTER_CRYPT = 16,
    EXIT = 17,
    NEWFD = 18
};

typedef struct Order_s{
    int orderNum;
    char orderTarget[ARGSLEN];
}Order_t, *pOrder_t;

typedef struct reOrder_s{
    int reOrderNum;
    char reInfo[1004];
}reOrder_t, *pReOrder_t;

typedef struct DataTrain_s{
    int len;
    char buf[1024];
}DataTrain_t,*pDataTrian_t;

int beginSystem(int sfd);
int order_load(int sfd, pOrder_t pOrder, pReOrder_t pReOrder);
int order_register(int sfd, pOrder_t pOrder, pReOrder_t pReOrder);
int send_Order(int sfd);
int changeOrder(char *initOrder, int *orderNum);
int read_Order(int sfd, pPool_t pPool);
char *genCipherText(char *salt, char *password);
char *genRandomString(void);

int putsFile(int sfd, pOrder_t pOrder);
int sendfile(pWork_t pWork);
int recvfile(pWork_t pWork);

#endif


