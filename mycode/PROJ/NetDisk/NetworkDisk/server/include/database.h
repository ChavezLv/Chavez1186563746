#ifndef __DATABASE_H__
#define __DATABASE_H__
#include <mysql/mysql.h>

MYSQL* mysqlConnect();
int mysqlInsert(MYSQL *conn, char *command);
int mysqlSearchInt(MYSQL *conn, char *command);
char* mysqlSearch(MYSQL *conn, char *command);
int mysqlClose(MYSQL *conn);

#endif
