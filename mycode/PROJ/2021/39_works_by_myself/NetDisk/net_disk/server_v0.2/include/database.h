#ifndef __DATABASE_H__
#define __DATABASE_H__
#include <mysql/mysql.h>

MYSQL* mysqlConnect();

int mysqlInsert(MYSQL* conn, char* command);

int mysql_update(char *query);

int mysqlSearch(MYSQL* conn, char* command, void* retStr);
int mysqlSearch2(MYSQL* conn, char* command, void** retStr);
/* char* mysqlSearch1(MYSQL* conn, char* command); */

/* int mysqlSearchInt(MYSQL* conn, char* command); */
/* char* mysqlSearch(MYSQL* conn, char * command); */
int mysqlDelete(MYSQL *conn, char* query);

int logIntoMysql(MYSQL * conn,int  userID,char *op);

int mysqlClose(MYSQL* conn);

#endif
