#include "../include/head.h"
#include "../include/database.h"

MYSQL* mysqlConnect() {
    MYSQL* mysqlConn;
    char* server = "localhost";
    char* user = "root";
    char* password = "123456";
    char* database = "net_work";

    mysqlConn = mysql_init(NULL);

    if (!mysql_real_connect(mysqlConn, server, user, password, database, 0, NULL, 0)) {
printf("?mysql>>Error connecting to database: %s\n", mysql_error(mysqlConn));
        return NULL;
    }
    return mysqlConn;
}

int mysqlInsert(MYSQL* conn, char* command) {
    puts(command);
    int queryResult = mysql_query(conn, command);
    if (queryResult) {
printf("?mysql>>Error query:%s\n", mysql_error(conn));
        return -1;
    }
    else {
printf("$mysql>>insert success\n");
    }
    return 0;
}
int mysqlSearch(MYSQL* conn, char* command, void* retStr)
{
    MYSQL_RES *res;
    MYSQL_ROW row;
    int queryRet = mysql_query(conn, command);
    if(queryRet)
    {
printf("?mysql>>Error query: %s\n", mysql_error(conn));
        strcpy((char*)retStr, mysql_error(conn));
        return -1;
    }
    else
    {
        res = mysql_store_result(conn);
/* printf("mysql_affacted_rows: %lu\n", (unsigned long)mysql_affected_rows(conn)); */
printf("$mysql>>mysql_num_rows: %lu\n", (unsigned long)mysql_num_rows(res));
        row = mysql_fetch_row(res);
        if(NULL == row){
            strcpy((char*)retStr, "0");
            return -1;
        }else{
            do
            {
printf("$mysql>>num row: %lu\n", (unsigned long)mysql_num_rows(res));
/* printf("num fileds: %lu\n", (unsigned long)mysql_num_fields(res)); */
                for(queryRet = 0; queryRet != (int)mysql_num_fields(res); ++queryRet)
                {
/* printf("%8s ", row[queryRet]); */
                    strcpy((char*)retStr, row[queryRet]);
                }
/* printf("\n"); */
            }while(NULL != (row = mysql_fetch_row(res)));
        }
        mysql_free_result(res);
    }
    return 0;
}
int mysqlSearchInt(MYSQL* conn, char* command) {
    MYSQL_RES* res;
    MYSQL_ROW row;
    int userID = 0;
    int queryRet = mysql_query(conn, command);
    if (queryRet)
    {
printf("?mysql>>Error query: %s\n", mysql_error(conn));
        return -1;
    }
    else
    {
        res = mysql_store_result(conn);
/* printf("mysql_affacted_rows: %lu\n", (unsigned long)mysql_affected_rows(conn)); */
printf("$mysql>>mysql_num_rows: %lu\n", (unsigned long)mysql_num_rows(res));

        row = mysql_fetch_row(res);
        if (NULL == row)
        {
printf("$mysql>>Don't query any data\n");

        }
        else
        {
            do
            {
printf("$mysql>>num row: %lu\n", (unsigned long)mysql_num_rows(res));
printf("$mysql>>num fileds: %lu\n", (unsigned long)mysql_num_fields(res));
                for (queryRet = 0; queryRet != (int)mysql_num_fields(res); ++queryRet)
                {
//printf("%8s ", row[queryRet]);
                    userID = atoi(row[queryRet]);
                }
//printf("\n");
            } while (NULL != (row = mysql_fetch_row(res)));
        }
        mysql_free_result(res);
    }
    return userID;
}

int mysql_delete(char *query)
{
	MYSQL *conn = mysqlConnect();

	int queryRet;

	queryRet = mysql_query(conn, query);
	if(queryRet)
	{
printf("$mysql>>Error making query:%s\n",mysql_error(conn));
        return -1;
	}
    else
    {
        unsigned long ret = mysql_affected_rows(conn);
        if(ret)
        {
printf("$mysql>>delete success, delete row=%lu\n", ret);
        }
        else
        {
printf("$mysql>>delete failed, mysql_affected_rows: %lu\n", ret);
            return -1;
        }
	}

	mysql_close(conn);

	return 0;
}

int mysql_update(char *query)
{
	MYSQL *conn = mysqlConnect();

	int queryResult;
	
    queryResult = mysql_query(conn, query);
	if(queryResult)
	{
printf("?mysql>>Error making query:%s\n",mysql_error(conn));
        return -1;
	}
    else
    {
        int ret = mysql_affected_rows(conn);
        if(ret)
        {
printf("$mysql>>update success\n");
        }
        else
        {
printf("?mysql>>update fail, mysql_affected_rows:%d\n", ret);
            return -1;
        }
	}
	mysql_close(conn);
	return 0;
}


//返回根据查询字段得到的结果
int mysqlClose(MYSQL* conn) {
    mysql_close(conn);
    return 0;
}

int logIntoMysql(int  userID, char* op)
{
    MYSQL* conn = mysqlConnect();
    char query[512] = { 0 };
    sprintf(query, "insert into LOG_OPERATION (userID, operation) values(%d,'%s')", userID, op);
printf("?mysql>>%s", query);
    mysqlInsert(conn, query);
    mysql_close(conn);
    return 0;
}
