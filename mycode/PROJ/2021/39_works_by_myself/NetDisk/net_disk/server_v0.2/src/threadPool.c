#include "../include/head.h"
#include "../include/thread_pool.h"
#include "../include/database.h"
#include "../include/encryption.h"
typedef struct Order_s{//客户端发送给服务端的命令
    /* int len; //数据总的长度 */
    //int userID;  //初始为-1, 注册用户名如果存在,则返回唯一用户id给客户端
    int orderNum;//命令类型
    //char step; // A a   B b  指定的某个命令的步骤, 客户端发过来为大写, 如果步骤没有完成, 则不改变大小写, 完成则改变
    //比如gets 第一步先确定自己id所在的文件是否存在
    char orderTarget[124];//存放命令所需的参数
}Order_t, *pOrder_t;

typedef struct reOrder_s{
    int retNum;//存放需发送的标志
    char retInfo[1000];//存放发给客户端的信息，如存放错误信息，ls存放要显示的内容，要求客户端接到能直接打印。
}reOrder_t, *pReOrder_t;

#define ARGSLEN_BIG 100
typedef struct work_Node_s{
    int type;
    int clientFd;
    int *Fd;
    char workArgs[ARGSLEN_BIG];
    struct work_Node_s *pNext; 
}Work_t, *pWork_t;


int judgeCommand(int clientFd);
int handleRegister(int clientFd);
int order_gets(pReOrder_t reOrder, char *orderTarget, int userId);
int handleRegisterRecvUserName(pReOrder_t reOrder, char* orderTarget, int userId);
int handleRegisterRecvCrypt(pReOrder_t reOrder, char* orderTarget, int userId);
int order_puts(pReOrder_t reOrder, pWork_t pWork, int userId);

int threadPoolInit(pThreadPool_t pPool, int threadNum, int capacity){
    queInit(&pPool->que, capacity); 
    
    pthread_cond_init(&pPool->cond, NULL);

    pPool->ptheadID = (pthread_t *)calloc(threadNum, sizeof(pthread_t));

    pPool->threadNum = threadNum;

    pPool->starFlag = '0';

    pPool->endFlag = '0';
    return 0;
}

void* threadFunc(void* p){
    pThreadPool_t pPool = (pThreadPool_t) p;
    pNode_t pGet = NULL;
    int ret = 0;
    while(1){
        pthread_mutex_lock(&pPool->que.que_mutex);
        /*这里while(0 == pPool->que.queSize)比if(0 == pPool->que.queSize)好点吧?避免虚假唤醒*/
        while(0 == pPool->que.queSize){
            pthread_cond_wait(&pPool->cond, &pPool->que.que_mutex); 
        }
        ret = queGet(&pPool->que, &pGet);
        pthread_mutex_unlock(&pPool->que.que_mutex);
        
        if(0 == ret){
            ret = judgeCommand(pGet->clientFd);
            PRINT_CHECK(ret);
            if(1 == ret){
                //注册
                /* handleRegister(pGet->clientFd); */
                while(1);
            }else if(3 == ret){
                transFile(pGet->clientFd);
                pReOrder_t reOrder = (pReOrder_t)calloc(1, sizeof(reOrder_t));
                /* order_gets(reOrder, (char*)"file", 35); */
                /* order_puts(reOrder, pWork,35); */
            }
            //传输结束,释放堆空间
            free(pGet);
            pGet = NULL;
        }
        //exit 
        PRINT_CHECK(pPool->endFlag);
        if('1' == pPool->endFlag){
            break;
        }
    }
    printf("one thread exit\n");
    pthread_exit(0);
}
#define MD5_SIZE 16
int order_puts(pReOrder_t reOrder, pWork_t pWork, int userId){
    int ret = 0;
    char filename[124] = { 0 };
    int filesize = 0;
    char md5Ret[MD5_SIZE] = { 0 };
    MYSQL* conn = mysqlConnect();

    Order_t order;
    memset(&order, 0, sizeof(order));
    //orderTarget文件名 
    recv(pWork->clientFd, &order, sizeof(order), 0);
    if(-1 == order.orderNum){
        //如果对方puts的filesize不存在
        return -1;
    }
    strcpy(filename, order.orderTarget);

    //如果对方puts的filesize存在,则开始接收
    /* //方案一: recv两次, long类型接收文件大小更大 */
    /* off_t filesize = 0; */
    /* memset(&order, 0, sizeof(order)); */
    /* recv(pWork->clientFd, &order, sizeof(order), 0); */
    /* strncpy((char*)&filesize, order.orderTarget, sizeof(off_t)); */
    /* strcpy(md5Ret, order.orderTarget + sizeof(off_t)); */
    
    //方案二: 正常做法, int类型限定了 接收文件最大为4G
    memset(&order, 0, sizeof(order));
    recv(pWork->clientFd, &order, sizeof(order), 0);
    filesize = order.orderNum;
    if(0 == filesize){
        //接收的文件大小为0, 则结束此次任务
        order.orderNum = 0;
        memset(&order, 0, sizeof(order));
        strcpy(order.orderTarget, "empty file!");
        send(pWork->clientFd, &order, sizeof(order), 0);
        
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
            send(pWork->clientFd, &order, sizeof(order), 0);
            
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
        send(pWork->clientFd, &order, sizeof(order), 0);
        return 0;
    }

    //终于到了终点: 全新的文件,开始下载
    order.orderNum = 1;
    send(pWork->clientFd, &order, sizeof(order), 0);
    recvFile(pWork->clientFd, filename, filesize);

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
int handleRegisterRecvUserName(pReOrder_t reOrder, char* orderTarget, int userId){
    //orderTargeti存放的是用户想要注册的用户名
    int ret;
    char *retSearch = (char *)calloc(1, 512);
    char query[512] = { 0 };
    MYSQL *mysqlConn = mysqlConnect();
        
    //查找是否有该用户
    sprintf(query, "select userName from USER_INFO where userName =\
            '%s'", orderTarget);
    /* puts(query); */
    /* retSearch = mysqlSearch(mysqlConn, query); */
    ret = mysqlSearch(mysqlConn, query, retSearch);
    if(-1 == ret){
        //如果用户名已经存在
        reOrder->retNum = -1;
        strcpy(reOrder->retInfo, "user name is already exist");
        return -1;
    }
    //没有注册过的用户名
    char *salt = (char* )calloc(1, SALT_LEN);
    salt = genRandomString(SALT_LEN);//after using char* type userName is changed to NULL

    //插入数据库
    memset(query, 0, sizeof(query)); 
    sprintf(query, "insert into USER_INFO (userName, salt, cryptpasswd, token, vipLevel)\
            values ('%s','%s', '0', '0', 0)\n", orderTarget, salt);
    mysqlInsert(mysqlConn, query);

    int userID = 0;
    memset(query, 0, sizeof(query)); 
    sprintf(query, "select userID from USER_INFO where userName = '%s'", orderTarget);
    puts(query);
    mysqlSearch(mysqlConn, query, &userID);

    //发送salt给客户
    reOrder->retNum = userID;
    strcpy(reOrder->retInfo, salt);

    free(salt);
    salt = NULL;
    free(retSearch);
    retSearch = NULL;
    return 0;
};

int handleRegisterRecvCrypt(pReOrder_t reOrder, char* orderTarget, int userId){
    char query[512] = { 0 };
    char userName[20] = { 0 };
    MYSQL *mysqlConn = mysqlConnect();

    //将密文orderTarget插入数据库, 
    memset(query, 0, sizeof(query)); 
    sprintf(query, "update USER_INFO set cryptpasswd = '%s' \
            where userID = %d ", orderTarget, userId);
    puts(query);
    mysqlInsert(mysqlConn, query);

    //在虚拟文件表DIR_INFO创建一条根目录
    //1)首先先获取用户名, 创建以根目录为用户名的目录
    memset(query, 0, sizeof(query));
    sprintf(query, "select userName from USER_INFO where userId = %d", userId);
    mysqlSearch(mysqlConn, query, userName);

    memset(query, 0, sizeof(query)); 
    sprintf(query, "insert into DIR_INFO (parentDirID, dirName, userID) values \
            (0, '%s',%d)", userName, userId);
    mysqlInsert(mysqlConn, query); 

    //2)根据创建的用户名为名的目录查目录id
    int dirID;
    memset(query, 0, sizeof(query)); 
    sprintf(query,"select dirID from DIR_INFO where userID = %d", userId);
    puts(query);
    mysqlSearch(mysqlConn, query, &dirID);
    //3)更改外键
    memset(query, 0, sizeof(query)); 
    sprintf(query, "update USER_INFO set pwdDirID = %d where userID = %d", dirID, userId);
    puts(query);
    mysqlInsert(mysqlConn, query);

    //4)注册成功标志
    reOrder->retNum = 0;
    strcpy(reOrder->retInfo, "register success");

    return 0;
}

int judgeCommand(int clientFd){
    int type = 0;
    DataTrain_t train;
    memset(&train, 0, sizeof(train));
    
    recv(clientFd, &train.len, sizeof(train.len), 0);
    /* PRINT_CHECK(train.len); */
    recv(clientFd, &train.buf, train.len, 0);
    
    if(0 == strcmp(train.buf, "1")){
        type = 1;//注册
    }
    
    return type;
}
int threadPoolStart(pThreadPool_t pPool){
    if('0' == pPool->starFlag){
        pPool->starFlag = '1';
        for(int i = 0; i< pPool->threadNum; ++i){
            int ret = pthread_create(&(pPool->ptheadID[i]), NULL, threadFunc, pPool);
            ERROR_CHECK(ret, -1, "pthread_create");
        }
    }

    return 0;
}
