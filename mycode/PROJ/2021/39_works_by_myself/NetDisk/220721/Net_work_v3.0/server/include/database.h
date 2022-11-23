#ifndef __DATABASE_H__
#define __DATABASE_H__
#include <mysql/mysql.h>

MYSQL* mysqlConnect();
int mysqlInsert(MYSQL *conn, char *command);
int mysql_delete(char *query);
int mysql_update(char *query);
int mysqlSearchInt(MYSQL *conn, char *command);
int mysqlSearch(MYSQL* conn, char* command, void* retStr);
int mysqlClose(MYSQL *conn);
int logIntoMysql(int  userID, char* op);

#endif
