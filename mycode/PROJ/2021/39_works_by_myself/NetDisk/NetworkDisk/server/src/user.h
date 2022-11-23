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
    //��������û����Ѿ�����ʱ
    do {
        //�����û���
        recv(clientFd, &train.len, 4, 0);
        recv(clientFd, train.buf, train.len, 0);
        //�����Ƿ��и��û�
        /* retSearch = mysqlSearch2(mysqlConn, (char*)"USER_INFO", (char*)"userName", train.buf); */
        sprintf(query, "select userName from USER_INFO where userName = '%s'", userName);
        retSearch = mysqlSearch(mysqlConn, query);

        train.len = 1;
        if (NULL != retSearch) {
            //����û����Ѿ�����,����һ������"0"��client
            strcpy(train.buf, "0");
        }
        else {
            memcpy(userName, train.buf, strlen(train.buf));
            strcpy(train.buf, "1");
        }
        send(clientFd, &train, 4 + train.len, 0);
    } while (NULL != retSearch);

    //û��ע������û���
    /* char salt[SALT_LEN] = { 0 }; */
    char* salt = (char*)calloc(1, SALT_LEN);
    salt = genRandomString(SALT_LEN);//after using char* type userName is changed to NULL
    /* printf("userName = %s, salt = %s, query = %s\n",userName, salt, query); */
    memset(query, 0, sizeof(query));
    sprintf(query, "insert into USER_INFO (userName, salt, cryptpasswd, token, vipLevel)\
            values ('%s','%s', '0', '0', 0)\n", userName, salt);
    //�������ݿ�
    mysqlInsert(mysqlConn, query);

    //����salt���ͻ�
    memset(&train.buf, 0, sizeof(train.buf));
    train.len = strlen(salt);
    strcpy(train.buf, salt);
    send(clientFd, &train, train.len + 4, 0);

    //��������
    memset(&train.buf, 0, sizeof(train.buf));
    recv(clientFd, &train.len, 4, 0);
    recv(clientFd, train.buf, train.len, 0);

    //�����Ĳ������ݿ�
    memset(query, 0, sizeof(query));
    sprintf(query, "update USER_INFO set cryptpasswd = '%s' where salt = '%s'", train.buf, salt);
    puts(query);
    mysqlInsert(mysqlConn, query);

    //����ע����ɸ��ͻ���
    strcpy(train.buf, "1");
    train.len = 1;
    send(clientFd, &train, train.len + 4, 0);

    //�������ļ���DIR_INFO����һ����Ŀ¼
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

//���������ֵ���ܣ���������
char *genCipherText(char *salt, char *password){
    //���salt��ͬ��password��ͬ����crypt()���ص��ַ�����ͬ
    char *cipherText = crypt(password, salt);
    return cipherText;
}

