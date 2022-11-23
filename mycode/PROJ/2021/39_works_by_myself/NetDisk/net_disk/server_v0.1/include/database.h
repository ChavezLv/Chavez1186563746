#ifndef __DATABASE_H__
#define __DATABASE_H__
#include <mysql/mysql.h>

MYSQL* mysqlConnect();
int mysqlInsert(MYSQL* conn, char* command);
/* char* mysqlSearch(MYSQL* mysqlConn, char *select, char* is); */
int mysqlSearchInt(MYSQL* conn, char* command);
char* mysqlSearch(MYSQL* conn, char* fromTable, char *selectCol, char* isWhich);
int mysqlClose(MYSQL* conn);

#endif
