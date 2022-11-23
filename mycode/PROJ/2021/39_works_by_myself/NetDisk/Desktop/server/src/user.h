#include"../include/orders.h"
int handleRegister(char *userName) {
    /* int ret; */
    DataTrain_t train;
    memset(&train, 0, sizeof(train));

    printf("begin registration\n");
    MYSQL* mysqlConn = mysqlConnect();

    char* retSearch = NULL;
    /* char userName[20] = { 0 }; */
    char* userName = (char*)calloc(1, 20);

    char query[2000] = { 0 };
    //当输入的用户名已经存在时
    do {
        //接收用户名
        recv(clientFd, &train.len, 4, 0);
        recv(clientFd, train.buf, train.len, 0);
        //查找是否有该用户
        /* retSearch = mysqlSearch2(mysqlConn, (char*)"USER_INFO", (char*)"userName", train.buf); */
        sprintf(query, "select userName from USER_INFO where userName = '%s'", userName);
        retSearch = mysqlSearch(mysqlConn, query);

        train.len = 1;
        if (NULL != retSearch) {
            //如果用户名已经存在,则发送一个错误"0"给client
            strcpy(train.buf, "0");
        }
        else {
            memcpy(userName, train.buf, strlen(train.buf));
            strcpy(train.buf, "1");
        }
        send(clientFd, &train, 4 + train.len, 0);
    } while (NULL != retSearch);

    //没有注册过的用户名
    /* char salt[SALT_LEN] = { 0 }; */
    char* salt = (char*)calloc(1, SALT_LEN);
    salt = genRandomString(SALT_LEN);//after using char* type userName is changed to NULL
    /* printf("userName = %s, salt = %s, query = %s\n",userName, salt, query); */
    memset(query, 0, sizeof(query));
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
    sprintf(query, "update USER_INFO set cryptpasswd = '%s' where salt = '%s'", train.buf, salt);
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
            (0, '%s',%d)", userName, userID);
    puts(query);
    mysqlInsert(mysqlConn, query);

    memset(query, 0, sizeof(query));
    sprintf(query, "select dirID from DIR_INFO where userID = %d", userID);
    puts(query);
    int dirID = mysqlSearchInt(mysqlConn, query);

    memset(query, 0, sizeof(query));
    sprintf(query, "update USER_INFO set pwdDirID = %d where userID = %d", dirID, userID);
    puts(query);
    mysqlInsert(mysqlConn, query);

    mysqlClose(mysqlConn);
    return 0;
}



char *genRandomString(int length){
    int flag, i;
    char *string;
    srand((unsigned)time(NULL));
    string = (char*)malloc(length);
    for(i = 0; i < length+1; ++i){
        flag = rand() % 3;
        switch(flag){
        case 0:
            string[i] = 'A' + rand() % 26;
            break;
        case 1:
            string[i] = 'a' + rand() % 26;
            break;
        case 2:
            string[i] = '0' + rand() % 10;
            break;
        default:
            string[i] = 'x';
            break;       
        }
    }
    string[length] = '\0';
    return string;
}

//将密码和盐值加密，生成密文
char *genCipherText(char *salt, char *password){
    //如果salt相同，password相同，则crypt()返回的字符串相同
    char *cipherText = crypt(password, salt);
    return cipherText;
}

