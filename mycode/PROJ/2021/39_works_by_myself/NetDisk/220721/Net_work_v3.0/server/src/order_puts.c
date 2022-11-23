#include "../include/orders.h"
#include "../include/md5.h"
#include"../include/trans.h"
//int order_puts(pReOrder_t pReOrder, char *orderTarget, int userId)
//{
//    sprintf(pReOrder->retInfo, "user %d's order: puts %s\n", userId, orderTarget);
//    return 0;
//}
/* int order_puts(pReOrder_t reOrder, pWork_t pWork, int userId){ */
int order_puts(pReOrder_t pReOrder, pWork_t pWork){
    
    pOrder_t pOrder = (pOrder_t)pWork->workArgs;
    char *orderTarget = pOrder->orderTarget;
    int *Fd = pWork->Fd;
    int clientFd = pWork->clientFd;
    int userId = Fd[clientFd];
    
    char filename[124] = { 0 };
    int filesize = 0;
    int fd;
    char md5Ret[64] = { 0 };
    MYSQL* conn = mysqlConnect();

    sscanf(orderTarget, "%s %s %d %d", md5Ret, filename, &filesize, &fd);
    if(0 >= filesize){
        pReOrder->retNum = -1;
        strcpy(pReOrder->retInfo, "path is wrong");
        return -1;
    }
    int dirIdInUserInfo = 0;
    int fileId = 0;
    char query[512] = { 0 };
    sprintf(query, "select pwdDirID from USER_INFO where userID = %d", userId);

    dirIdInUserInfo = mysqlSearchInt(conn, query);
    bzero(query, sizeof(query)); 
    sprintf(query, "select fileID from USER_FILE where dirID = %d and fileName = '%s'", \
            dirIdInUserInfo, filename);
    fileId = mysqlSearchInt(conn, query);
    if(fileId != -1){
        pReOrder->retNum = -1;
        strcpy(pReOrder->retInfo, "repeat filename");
        return -1;
    }
printf("dirId:%d userId: %d fileId: %d\n", dirIdInUserInfo, userId, fileId);
    //dirId如果不一样则直接查找全局md5
    //查询文件表
    bzero(query, sizeof(query));
    sprintf(query, "select fileId from FILE_INFO where MD5 = '%s'", md5Ret); 
    fileId = mysqlSearchInt(conn, query);
    if(-1 != fileId){
        //如果相等的md5值
        bzero(query, sizeof(query));
        sprintf(query, "insert into USER_FILE (dirID, userID, fileID, fileName) values (%d, %d, %d, '%s')", dirIdInUserInfo, userId, fileId, filename); 
        mysqlInsert(conn, query);

        bzero(query, sizeof(query));
        sprintf(query, "update FILE_INFO set referCnt = referCnt + 1 where fileId = %d", fileId);
        mysql_update(query);     
    
        pReOrder->retNum = 0;
        strcpy(pReOrder->retInfo, "puts success!");
        return 0;
    }

    pWork_t pNewWork = (pWork_t)calloc(1, sizeof(Work_t));
    
    pOrder_t pOrder_med = (pOrder_t)calloc(1, sizeof(Order_t));
    pOrder_med->orderNum = RECVFILE;
    sprintf(pOrder_med->orderTarget, "%s %s %d %d %d" , md5Ret, filename, filesize, userId, dirIdInUserInfo);
    memcpy(pNewWork->workArgs, pOrder_med, sizeof(Order_t));
    free(pOrder_med);
    
    pNewWork->clientFd = clientFd;
    pNewWork->Fd = Fd;
    pNewWork->pPool = pWork->pPool;
    
    pReOrder->retNum = RECVFILE;
    sprintf(pReOrder->retInfo, "%d %p %d", Fd[clientFd], pNewWork, fd);
    
    char op[1024] = {0};
    sprintf(op, "puts %s", orderTarget);
    logIntoMysql(userId, op);
    return 0;
}
