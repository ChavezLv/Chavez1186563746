#ifndef __ORDERS_H__
#define __ORDERS_H__
#include"../include/head.h"
#include"../include/work_Queue.h"
#include<crypt.h>
#define MAX 10
#define SALT_LEN 2
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
    LOAD_USERNAME = 11,
    LOAD_CRYPT = 12,
    REGISTER_USERNAME = 13,
    REGISTER_CRYPT = 14,
    EXIT = 15
};

typedef struct Order_s{
    int orderNum;
    char orderTarget[ARGSLEN];
}Order_t, *pOrder_t;

typedef struct reOrder_s{
    int reOrderNum;
    char reInfo[1024];
}reOrder_t, *pReOrder_t;

int beginSystem(int sfd);
int order_load(int sfd, pOrder_t pOrder, pReOrder_t pReOrder);
int order_register(int sfd, pOrder_t pOrder, pReOrder_t pReOrder);
int send_Order(int sfd);
int changeOrder(char *initOrder, int *orderNum);
int read_Order(int sfd);
char *genCipherText(char *salt, char *password);
char *genRandomString(int length);



int putsFile(int sfd, char *filename);
int transFile(int fd, int sfd, int filesize);

#endif


