#include <mysql/mysql.h>
#include <string.h>
#include <stdio.h>
/* #pragma comment(lib, "libmysql.lib") */

int main(int argc,char* argv[])
{
	MYSQL *conn;
	char *server = "localhost";
	char *user = "root";
	char *password = "";
	char *database = "pra";//要访问的数据库名称
    char query[200]="insert into hero (id, name, age) values (27,'zhaoxin', 78)";
    /* MYSQL_RES *res; */
    /* MYSQL_ROW row; */
    /* char query[200]="select * from hero"; */
	int queryResult;

	conn = mysql_init(NULL);

	if(!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
	{
		printf("Error connecting to database: %s\n", mysql_error(conn));
	}
    else
    {
		printf("Connected...\n");
	}
    
    /* res = mysql_store_result(&conn); */
	queryResult = mysql_query(conn, query);
	if(queryResult)
	{
		printf("Error making query:%s\n", mysql_error(conn));
	}
    else
    {
		printf("insert success\n");
	}
	mysql_close(conn);

	return 0;
}
