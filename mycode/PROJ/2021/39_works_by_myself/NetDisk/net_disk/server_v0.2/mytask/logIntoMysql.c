#include "../include/head.h"
#include "../include/database.h"

/* int logIntoMysql(MYSQL * conn,int  userID,char *op){ */
/*     char query[512] = { 0 }; */
/*     sprintf(query, "insert into LOG_OPERATION (userID, operation) values(%d,'%s')", userID, op); */
/*     mysqlInsert(conn, query); */
/*     return 0; */
/* } */
int main(int argc,char*argv[]){
    MYSQL * conn = mysqlConnect();
    /* logIntoMysql(conn, 0, (char*)"logout"); */
    char query[512] = { 0 };
    /* char retStr[512] = { 0 }; */
    /* int retStr[10][10]; /1* char (*retStr)[512] = (char (*)[512]) calloc(10, 512); *1/ */
    int retStr[10] = { 0 };
    /* sprintf(query, "select dirID from DIR_INFO where userID = %d", 25); */
    /* sprintf(query, "select userID from USER_INFO where userName = 'xiaojiu'"); */
    sprintf(query, "select userID from USER_INFO");
    /* sprintf(query, "select userID from USER_INFO where userID > 1 "); */
    puts(query);
    /* strcpy(retStr, mysqlSearch1(conn, query)); */
    int ret = mysqlSearch2(conn, query, (void**)&retStr);
    if(ret != -1){
        for(int i = 0; i < 10; ++i){
            printf("%d\n", retStr[i]);
        }    
    }else{
        printf("已经存在");
    }
    /* mysqlSearch1(conn, query, retStr); */
    /* for(int i = 0; retStr[i] != NULL; ++i){ */
        /* puts(retStr[i]); */
    /* } */
    return 0;
}

