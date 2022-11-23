#include"../include/orders.h"
char *genRandomString(void)
{
    int flag, i;
    srand((unsigned)time(NULL));
    char *string = (char*)malloc(RANDOM_STR_LEN);
    char *salt = (char*)malloc(SALT_LEN);
    bzero(salt, SALT_LEN);
    
    for(i = 0; i < RANDOM_STR_LEN+1; ++i){
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
    string[RANDOM_STR_LEN] = '\0';
    sprintf(salt, "$6$%s", string);
    return salt;
}

//���������ֵ���ܣ���������
char *genCipherText(char *salt, char *password){
    //���salt��ͬ��password��ͬ����crypt()���ص��ַ�����ͬ
    char *cipherText = crypt(password, salt);
    return cipherText;
}

int userName(pReOrder_t reOrder, char *orderTarget)
{
	//orderTarget������username
	int ret = 0;
	//�����û���
	char username[24] = { 0 };
	strcpy(username, orderTarget);
	char query[200] = { 0 };
	sprintf(query, "select salt from USER_INFO where userName = '%s'", username);
printf("query=%s\n", query);
	MYSQL *conn = mysqlConnect();//�ǵ�close
	char reStr[1000] = { 0 };
	ret = mysqlSearch(conn, query, reStr);//��ѯ�������д��
	if (ret == -1)
	{
		reOrder->retNum = -1;
		strcpy(reOrder->retInfo, "the userName is wrong");
		return -1;
	}
	strcpy(reOrder->retInfo, reStr);
	memset(query, 0, sizeof(query));
	memset(reStr, 0, sizeof(reStr));
	sprintf(query, "%s%s%s", "select userID from USER_INFO where userName= '", username, "'");
	printf("query=%s\n", query);
	ret = mysqlSearch(conn, query, reStr);
	mysql_close(conn);
	reOrder->retNum = 0;
	return atoi(reStr);
}

int userCrypt(pReOrder_t reOrder, char *orderTarget, int userId)
{
	/*��������*/
	MYSQL *conn = mysqlConnect();
	int ret = 0;
	char query[200] = { 0 };
	char reStr[1000] = { 0 };
	memset(query, 0, sizeof(query));
	sprintf(query, "%s%d", "select cryptpasswd from USER_INFO where userID = ", userId);
	printf("query=%s\n", query);
	ret = mysqlSearch(conn, query, reStr);
	if (ret == -1)
	{
		reOrder->retNum = -1;
		strcpy(reOrder->retInfo, "the password search is wrong");
		return -1;
	}
	else
	{
		if (strcmp(reStr, orderTarget) == 0)
		{
			reOrder->retNum = 0;
			strcpy(reOrder->retInfo, "the password is correct");
		}
		else
		{
			reOrder->retNum = -1;
			strcpy(reOrder->retInfo, "the password is wrong");
			return -1;
		}
	}
	mysql_close(conn);
    char op[1024] = {0};
    sprintf(op, "user load");
    logIntoMysql(userId, op);
	return 0;
}


int handleRegisterRecvUserName(pReOrder_t reOrder, char* orderTarget)
{
    //orderTargeti��ŵ����û���Ҫע����û���
    int ret;
    char *retSearch = (char *)calloc(1, 512);
    char query[512] = { 0 };
    MYSQL *mysqlConn = mysqlConnect();
        
    //�����Ƿ��и��û�
    sprintf(query, "select userName from USER_INFO where userName =\
            '%s'", orderTarget);
    /* puts(query); */
    /* retSearch = mysqlSearch(mysqlConn, query); */
    ret = mysqlSearch(mysqlConn, query, retSearch);
    if(0 == ret){
        //����û����Ѿ�����
        reOrder->retNum = -1;
        strcpy(reOrder->retInfo, "user name is already exist");
        return -1;
    }
    //û��ע������û���
    char *salt = (char* )calloc(1, SALT_LEN);
    salt = genRandomString();//after using char* type userName is changed to NULL

    //�������ݿ�
    memset(query, 0, sizeof(query)); 
    sprintf(query, "insert into USER_INFO (userName, salt, cryptpasswd, token, vipLevel)\
            values ('%s','%s', '0', '0', 0)\n", orderTarget, salt);
    mysqlInsert(mysqlConn, query);

    memset(query, 0, sizeof(query)); 
    sprintf(query, "select userID from USER_INFO where userName = '%s'", orderTarget);
    puts(query);
    int userId = mysqlSearchInt(mysqlConn, query); 

    //����salt���ͻ�
    strcpy(reOrder->retInfo, salt);

    free(salt);
    salt = NULL;
    free(retSearch);
    retSearch = NULL;
    return userId;
};

int handleRegisterRecvCrypt(pReOrder_t reOrder, char* orderTarget, int userId)
{
    char query[512] = { 0 };
    char userName[20] = { 0 };
    MYSQL *mysqlConn = mysqlConnect();

    //������orderTarget�������ݿ�, 
    memset(query, 0, sizeof(query)); 
    sprintf(query, "update USER_INFO set cryptpasswd = '%s' \
            where userID = %d ", orderTarget, userId);
    puts(query);
    mysqlInsert(mysqlConn, query);

    //�������ļ���DIR_INFO����һ����Ŀ¼
    //1)�����Ȼ�ȡ�û���, �����Ը�Ŀ¼Ϊ�û�����Ŀ¼
    memset(query, 0, sizeof(query));
    sprintf(query, "select userName from USER_INFO where userId = %d", userId);
    mysqlSearch(mysqlConn, query, userName);

    memset(query, 0, sizeof(query)); 
    sprintf(query, "insert into DIR_INFO (parentDirID, dirName, userID) values \
            (0, '%s',%d)", userName, userId);
    mysqlInsert(mysqlConn, query); 

    //2)���ݴ������û���Ϊ����Ŀ¼��Ŀ¼id
    memset(query, 0, sizeof(query)); 
    sprintf(query,"select dirID from DIR_INFO where userID = %d", userId);
    puts(query);
    int dirID = mysqlSearchInt(mysqlConn, query);
    //3)�������
    memset(query, 0, sizeof(query)); 
    sprintf(query, "update USER_INFO set pwdDirID = %d where userID = %d", dirID, userId);
    puts(query);
    mysqlInsert(mysqlConn, query);

    //4)ע��ɹ���־
    reOrder->retNum = 0;
    strcpy(reOrder->retInfo, "register success");
    char op[1024] = {0};
    sprintf(op, "user register");
    logIntoMysql(userId, op);

    return 0;
}

