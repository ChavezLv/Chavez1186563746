#include "../include/orders.h"
#include "../include/md5.h"
#include "../include/orders.h"
//int order_puts(pReOrder_t pReOrder, char *orderTarget, int userId)
//{
//    sprintf(pReOrder->retInfo, "user %d's order: puts %s\n", userId, orderTarget);
//    return 0;
//}
/* int order_puts(pReOrder_t reOrder, pWork_t pWork, int userId){ */
int order_puts(pReOrder_t reOrder, int clientFd, int userId){
    int ret = 0;
    char filename[124] = { 0 };
    int filesize = 0;
    char md5Ret[MD5_SIZE] = { 0 };
    MYSQL* conn = mysqlConnect();

    Order_t order;
    memset(&order, 0, sizeof(order));
    //orderTarget文件名 
    recv(clientFd, &order, sizeof(order), 0);
    if(-1 == order.orderNum){
        //如果对方puts的filesize不存在
        return -1;
    }
    strcpy(filename, order.orderTarget);

    //如果对方puts的filesize存在,则开始接收
    /* //方案一: recv两次, long类型接收文件大小更大 */
    /* off_t filesize = 0; */
    /* memset(&order, 0, sizeof(order)); */
    /* recv(clientFd, &order, sizeof(order), 0); */
    /* strncpy((char*)&filesize, order.orderTarget, sizeof(off_t)); */
    /* strcpy(md5Ret, order.orderTarget + sizeof(off_t)); */
    
    //方案二: 正常做法, int类型限定了 接收文件最大为4G
    memset(&order, 0, sizeof(order));
    recv(clientFd, &order, sizeof(order), 0);
    filesize = order.orderNum;
    if(0 == filesize){
        //接收的文件大小为0, 则结束此次任务
        order.orderNum = 0;
        memset(&order, 0, sizeof(order));
        strcpy(order.orderTarget, "empty file!");
        send(clientFd, &order, sizeof(order), 0);
        
        return 0;
    }
    strcpy(md5Ret, order.orderTarget);
    
    //查找数据库
    //1) 查找USER_INFO有没有对应的文件id , 比对md5是否一致
    int dirIdInUserInfo = 0;
    int dirIdInUserFile = 0;
    int fileId = 0;
    char query[512] = { 0 };
    sprintf(query, "select pwdDirID from USER_INFO where userID = %d", userId);
    mysqlSearch(conn, query, &dirIdInUserInfo);
    bzero(query, sizeof(query)); 
    sprintf(query, "select dirID from USER_FILE where userID = %d", userId);
    mysqlSearch(conn, query, &dirIdInUserFile);

    char md5Search[16] = { 0 }; 
    if(dirIdInUserInfo == dirIdInUserFile && userId != 0){
        //dirId如果不一样则直接查找全局md5
        bzero(query, sizeof(query));
        sprintf(query, "select fileID from USER_FILE where userID = %d", userId);
        mysqlSearch(conn, query, &fileId);

        bzero(query, sizeof(query));
        sprintf(query, "select MD5 from FILE_INFO where fileId = %d", fileId);
        ret = mysqlSearch(conn, query, md5Search);
        if(0 == strcmp(md5Search,md5Ret)){
            //一样则返回, 结束此任务,如果不一样则考虑删除,或者重传?
            order.orderNum = 0;
            memset(&order, 0, sizeof(order));
            strcpy(order.orderTarget, "already exist!");
            send(clientFd, &order, sizeof(order), 0);
            
            return 0;
        }else if(0 == ret){//返回0表示有查到内容
            //md5不一样,采用方案时先删除, 再重新检查上传
            //先删除, 再删除
            bzero(query, sizeof(query));
            sprintf(query, "delete from USER_FILE where fileID = %d", fileId);
            mysqlDelete(conn, query);

            bzero(query, sizeof(query));
            sprintf(query, "delete from FILE_INFO where fileID = %d", fileId);
            mysqlDelete(conn, query);
        }
    }
    //查询文件表
    bzero(query, sizeof(query));
    sprintf(query, "select fileId from FILE_INFO where MD5 = '%s'", md5Ret); 
    ret = mysqlSearch(conn, query, &fileId);
    /* if(0 == ret){ */
    if(0 == strcmp(md5Ret,md5Search)){
        //如果相等的md5值
        bzero(query, sizeof(query));
        sprintf(query, "insert into USER_FILE (%d, %d, %d, '%s')", userId, dirIdInUserInfo, fileId, filename); 
        mysqlInsert(conn, query);

        bzero(query, sizeof(query));
        sprintf(query, "update FILE_INFO set referCnt = referCnt + 1 where fileId = %d", fileId);
        mysql_update(query);     
    
        //回复客户端
        order.orderNum = 0;
        memset(&order, 0, sizeof(order));
        strcpy(order.orderTarget, "puts success!");
        send(clientFd, &order, sizeof(order), 0);
        return 0;
    }

    //终于到了终点: 全新的文件,开始下载
    order.orderNum = 1;
    send(clientFd, &order, sizeof(order), 0);
    recvFile(clientFd, md5Ret, filesize);

    //插入一条记录
    bzero(query, sizeof(query));
    sprintf(query, "insert into FILE_INFO (filesize, MD5, referCnt) \
            values (%d, '%s', 1)", filesize, md5Ret);
    mysqlInsert(conn, query);
    
    bzero(query, sizeof(query));
    sprintf(query, "select fileID from FILE_INFO where MD5 = '%s'", md5Ret);
    mysqlSearch(conn, query, &fileId);//此时的fileId是新插入的文件的

    bzero(query, sizeof(query));
    sprintf(query, "insert into USER_FILE values (%d, %d, %d, '%s')", \
            userId, dirIdInUserInfo, fileId, filename);
    mysqlInsert(conn, query);

    return 0;
}
