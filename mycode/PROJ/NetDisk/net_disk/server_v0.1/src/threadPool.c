#include "../include/head.h"
#include "../include/thread_pool.h"
#include "../include/database.h"
#include "../include/encryption.h"

int judgeCommand(int clientFd);
int handleRegister(int clientFd);
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
                handleRegister(pGet->clientFd);
                while(1);
            }else if(3 == ret){
                transFile(pGet->clientFd);
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
int handleRegister(int clientFd){
    /* int ret; */
    DataTrain_t train;
    memset(&train, 0, sizeof(train));

    printf("begin registration\n");
    MYSQL *mysqlConn = mysqlConnect();

    char *retSearch = NULL;
    /* char userName[20] = { 0 }; */
    char *userName = (char *)calloc(1, 20);
    //当输入的用户名已经存在时
    do{
        //接收用户名
        recv(clientFd, &train.len, 4, 0);
        recv(clientFd, train.buf, train.len, 0);
        //查找是否有该用户
        retSearch = mysqlSearch(mysqlConn, (char*)"USER_INFO", (char*)"userName", train.buf); 

        train.len = 1;
        if(NULL != retSearch){
            //如果用户名已经存在,则发送一个错误"0"给client
            strcpy(train.buf, "0");
        }else{
            memcpy(userName, train.buf, strlen(train.buf));
            strcpy(train.buf, "1");
        }
        send(clientFd, &train, 4 + train.len, 0);
    }while(NULL != retSearch);

    //没有注册过的用户名
    /* char salt[SALT_LEN] = { 0 }; */
    char *salt = (char* )calloc(1, SALT_LEN);
    salt = genRandomString(SALT_LEN);//after using char* type userName is changed to NULL
    char query[2000] = { 0 };
    /* printf("userName = %s, salt = %s, query = %s\n",userName, salt, query); */
    sprintf(query, "insert into USER_INFO (userName, salt, cryptpasswd, token, vipLevel)\
            values ('%s','%s', '0', '0', 0)\n", userName, salt);
    //插入数据库
    mysqlInsert(mysqlConn, query);

    //发送salt给客户
    memset(&train.buf, 0, sizeof(train.buf));
    train.len = strlen(salt);
    strcpy(train.buf, salt);
    send(clientFd, &train, train.len + 4, 0);

    //接收密文
    memset(&train.buf, 0, sizeof(train.buf));
    recv(clientFd, &train.len, 4, 0);
    recv(clientFd, train.buf, train.len, 0);

    //将密文插入数据库
    memset(query, 0, sizeof(query)); 
    sprintf(query, "update USER_INFO set cryptpasswd = '%s' where salt = '%s'",train.buf, salt);
    puts(query);
    mysqlInsert(mysqlConn, query);

    //发送注册完成给客户端
    strcpy(train.buf, "1");
    train.len = 1;
    send(clientFd, &train, train.len + 4, 0);

    //在虚拟文件表DIR_INFO创建一条根目录
    memset(query, 0, sizeof(query)); 
    sprintf(query, "select userID from USER_INFO where userName = '%s'", userName);
    puts(query);
    int userID = mysqlSearchInt(mysqlConn, query); 

    memset(query, 0, sizeof(query)); 
    sprintf(query, "insert into DIR_INFO (parentDirID, dirName, userID) values \
            (0, '%s',%d)",userName, userID);
    puts(query);
    mysqlInsert(mysqlConn, query); 

    memset(query, 0, sizeof(query)); 
    sprintf(query,"select dirID from DIR_INFO where userID = %d", userID);
    puts(query);
    int dirID = mysqlSearchInt(mysqlConn, query);
    
    memset(query, 0, sizeof(query)); 
    sprintf(query, "update USER_INFO set pwdDirID = %d where userID = %d", dirID, userID);
    puts(query);
    mysqlInsert(mysqlConn, query);

    mysqlClose(mysqlConn);
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
