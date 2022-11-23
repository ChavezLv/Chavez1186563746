typedef struct
{
	int dataLen1;
	int dataLen2;
	char name[50];
	char pwd[50];
}log_t;
typedef struct
{
	int userID;
	char username[20];
	char salt[2];
	char cryptpasswd[13];
	char token[255];
	int vipLevel;
	int pwdDirID;
}User_t, *pUser_t;
MYSQL* mysqlConnect()
{
	MYSQL *mysqlConn;
	char *server = "localhost";
	char *user = "root";
	char *password = "123456";
	char *database = "networkDisk";//要访问的数据库

	//初始化
	conn = mysql_init(NULL);
	if (!conn)
	{
		printf("MySQL init failed\n");
		return -1;
	}

	//连接数据库看连接是否成功
	if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
	{
		printf("Error connecting to database:%s\n", mysql_error(conn));
		return NULL;
	}
	else
	{
		printf("MySQL connected correctly...\n");
		return mysqqlConn;
	}
}
int mysql_query1(char *query, char **result)
{
	MYSQL *conn = mysqlConnect();
	MYSQL_RES *res;
	MYSQL_ROW row;
	//while(1);
	unsigned int queryResult;
	queryResult = mysql_query(conn, query);
	//printf("queryResult=%d\n", queryResult);
	if (queryResult)
	{
		//查询语句没有问题
		printf("Error making query:%s\n", mysql_error(conn));
	}
	else {
		//query为0时语句查询成功
		printf("用户存在\n");
		res = mysql_store_result(conn);
		printf("mysql_num_rows=%lu\n", (unsigned long)mysql_num_rows(res));

		row = mysql_fetch_row(res);
		if (NULL == row)
		{
			printf("find but no data\n");
		}
		else
		{
			//行数大于0，查询成功，打印查询的结果
			do
			{
				for (queryResult = 0; queryResult < mysql_num_fields(res); ++queryResult)
				{

					strcat(*result, row[queryResult]);
					strcat(*result, " ");
					//printf("result = %s\n", *result);
				}
			} while (NULL != (row = mysql_fetch_row(res)));
		}
		mysql_free_result(res);
	}

	mysql_close(conn);
	//printf("数据库关闭准备返回\n");
	return 0;
}

pUser_t user_login(int clientfd, log_t *u)
{
	recv(clientfd, &u->dataLen1, 4, 0);
	/*接收密码的长度
	/printf("dataLen1=%d\n", dataLen1);*/
	recv(clientfd, &u->dataLen2, 4, 0);
	/*printf("dataLen2=%d\n", dataLen2);
	接收用户名*/
	recv(clientfd, u->usrName, 50, MSG_WAITALL);
	/*printf("userName=%s\n", usrName);
	/接收用户密码*/
	recv(clientfd, u->passWord, 50, MSG_WAITALL);
	/*printf("passWord=%s\n", passWord);*/
	char query[200] = { 0 };
	strcpy(query, "select * from USER_INFO where userName ='");
	sprintf(query, "%s%s%s", query, usrName, "'");
	printf("query=%s\n", query);
	int ret = 0;
	char *result = (char *)calloc(1, 128);
	ret = mysql_query1(query, &result);//查询结果允许写入
	if (ret == -1)
	{
		send(clientfd, &ret, sizeof(int), 0);
		return NULL;
	}
	else
	{
		printf("transFile find\n");
		pUser_t t = (pUser_t)calloc(1, sizeof(User_t));
		sscanf(result, "%d %s %s %s %s%d%d", &t->userID, t->username, t->salt, t->cryptpasswd, t->token, &t->vipLevel, &t->pwdDirID);
		if (strcmp(t->cryptpasswd, genCipherText(t->salt, u->pwd));
		{
			return t;
		}
		//printf("%d\n", t->userID);
		//printf("%s\n", t->username);
	}
	return NULL;
}