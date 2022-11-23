#include"mysql/mysql.h"
MYSQL* mysqlConnect();
int mysqlSearch(MYSQL* conn, char* command, void* result);
int mysqlInsert(MYSQL* conn, char* command);
