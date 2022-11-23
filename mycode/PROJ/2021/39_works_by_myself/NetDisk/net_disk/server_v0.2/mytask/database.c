#include "../include/head.h"
#include "../include/database.h"

MYSQL* mysqlConnect(){
    MYSQL *mysqlConn;
    char *server="localhost";
    char *user="root";
    char *password="123456";
    char *database="net_work";

    mysqlConn=mysql_init(NULL);

    if(!mysql_real_connect(mysqlConn,server,user,password,database,0,NULL,0)){
        printf("Error connecting to database: %s\n", mysql_error(mysqlConn));
        return NULL; 
    }
    return mysqlConn;
}

int mysqlInsert(MYSQL* conn, char* command){
    puts(command);
    int queryResult = mysql_query(conn, command);
    if(queryResult){
        printf("Error making query:%s\n", mysql_error(conn));
    }else{
        printf("insert success\n");
    }
    return 0;
}
/* char* mysqlSearch10(MYSQL* conn, char* command){ */
/*     MYSQL_RES *res; */
/*     MYSQL_ROW row; */
/*     char *retStr = (char*) calloc(1, 512); */
/*     /1* char retStr[512] = { 0 }; *1/ */
/*     int queryRet = mysql_query(conn, command); */
/*     if(queryRet) */
/*     { */
/*         printf("Error query: %s\n", mysql_error(conn)); */
/*         return NULL; */
/*     } */
/*     else */
/*     { */
/*         res = mysql_store_result(conn); */
/*         /1* printf("mysql_affacted_rows: %lu\n", (unsigned long)mysql_affected_rows(conn)); *1/ */
/*         /1* printf("mysql_num_rows: %lu\n", (unsigned long)mysql_num_rows(res)); *1/ */
/*         row = mysql_fetch_row(res); */
/*         if(NULL == row){ */
/*             /1* strcpy((char*)retStr, "Don't query any data"); *1/ */
/*         }else{ */
/*             do */
/*             { */
/*                 /1* printf("num row: %lu\n", (unsigned long)mysql_num_rows(res)); *1/ */
/*                 /1* printf("num fileds: %lu\n", (unsigned long)mysql_num_fields(res)); *1/ */
/*                 for(queryRet = 0; queryRet != (int)mysql_num_fields(res); ++queryRet) */
/*                 { */
/*                     printf("%8s ", row[queryRet]); */
/*                     strcpy((char*)retStr, row[queryRet]); */
/*                 } */
/*                 printf("\n"); */
/*             }while(NULL != (row = mysql_fetch_row(res))); */
/*         } */
/*         mysql_free_result(res); */
/*     } */
/*     return 0; */
/* } */
int mysqlSearch(MYSQL* conn, char* command, void* retStr){
    MYSQL_RES *res;
    MYSQL_ROW row;
    int queryRet = mysql_query(conn, command);
    if(queryRet)
    {
        /* printf("Error query: %s\n", mysql_error(conn)); */
        strcpy((char*)retStr, mysql_error(conn));
        return -1;
    }
    else
    {
        res = mysql_store_result(conn);
        /* printf("mysql_affacted_rows: %lu\n", (unsigned long)mysql_affected_rows(conn)); */
        /* printf("mysql_num_rows: %lu\n", (unsigned long)mysql_num_rows(res)); */
        row = mysql_fetch_row(res);
        if(NULL == row){
            strcpy((char*)retStr, "0");
            return 0;
        }else{
            do
            {
                /* printf("num row: %lu\n", (unsigned long)mysql_num_rows(res)); */
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

int mysqlSearch2(MYSQL* conn, char* command, void** retStr){
    MYSQL_RES *res;
    MYSQL_ROW row;
    int queryRet = mysql_query(conn, command);
    if(queryRet)
    {
        /* printf("Error query: %s\n", mysql_error(conn)); */
        /* strcpy((char*)retStr, mysql_error(conn)); */
        return -1;
    }
    else
    {
        res = mysql_store_result(conn);
        /* printf("mysql_affacted_rows: %lu\n", (unsigned long)mysql_affected_rows(conn)); */
        /* printf("mysql_num_rows: %lu\n", (unsigned long)mysql_num_rows(res)); */
        row = mysql_fetch_row(res);
        if(NULL == row){
            /* strcpy((char*)retStr, "Don't query any data"); */
            return 0;
        }else{
            do
            {
                /* printf("num row: %lu\n", (unsigned long)mysql_num_rows(res)); */
                /* printf("num fileds: %lu\n", (unsigned long)mysql_num_fields(res)); */
                int i = 0;
                for(queryRet = 0; queryRet != (int)mysql_num_fields(res); ++queryRet)
                {
                    printf("%8s ", row[queryRet]);
                    /* strcpy((char*)retStr[i], row[queryRet]); */
                    /* ((int*)retStr)[i] = atoi(row[queryRet]);//一维数组也是这么写 */
                    printf("int : %d \n", atoi(row[queryRet]));
                    (*(int**)retStr)[i] = atoi(row[queryRet]);
                    printf("int : %d \n", ((int*)retStr)[i]);
                    ++i;
                }
            }while(NULL != (row = mysql_fetch_row(res)));
                printf("\n");
            for(int i = 0; i < 10 ; ++i){
                printf("int : %d", ((int*)retStr)[i] );
            }
                printf("\n");
        }
        mysql_free_result(res);
    }
    return 0;
}
int mysqlSearchInt(MYSQL* conn, char* command){
    MYSQL_RES *res;
    MYSQL_ROW row;
    int userID = 0;
    int queryRet = mysql_query(conn, command);
    if(queryRet)
    {
        printf("Error query: %s\n", mysql_error(conn));
        return -1;
    }
    else
    {
        res = mysql_store_result(conn);
        /* printf("mysql_affacted_rows: %lu\n", (unsigned long)mysql_affected_rows(conn)); */
        printf("mysql_num_rows: %lu\n", (unsigned long)mysql_num_rows(res));

        row = mysql_fetch_row(res);
        if(NULL == row)
        {
            printf("Don't query any data\n");

        }
        else
        {
            do
            {
                printf("num row: %lu\n", (unsigned long)mysql_num_rows(res));
                printf("num fileds: %lu\n", (unsigned long)mysql_num_fields(res));
                for(queryRet = 0; queryRet != (int)mysql_num_fields(res); ++queryRet)
                {
                    printf("%8s ", row[queryRet]);
                    userID = atoi(row[queryRet]);
                }
                printf("\n");
            }while(NULL != (row = mysql_fetch_row(res)));
        }
        mysql_free_result(res);
    }
    return userID;
}
//返回根据查询字段得到的结果
/* char* mysqlSearch(MYSQL* conn, char * command){ */
/*     MYSQL_RES *res; */
/*     MYSQL_ROW row; */
/*     char *retStr = (char*) calloc(1, 300); */
/*     unsigned int queryRet; */
/*     queryRet = mysql_query(conn, command); */
/*     if(queryRet) */
/*     { */
/*         printf("Error query: %s\n", mysql_error(conn)); */
/*         return NULL; */
/*     } */
/*     else */
/*     { */
/*         res = mysql_store_result(conn); */
/*         /1* printf("mysql_affacted_rows: %lu\n", (unsigned long)mysql_affected_rows(conn)); *1/ */
/*         printf("mysql_num_rows: %lu\n", (unsigned long)mysql_num_rows(res)); */

/*         row = mysql_fetch_row(res); */
/*         if(NULL == row) */
/*         { */
/*             /1* printf("query empty data\n"); *1/ */
/*             printf("empty set\n"); */
/*             return NULL; */
/*         }else{ */
/*             /1* for(queryRet = 0; queryRet != (int)mysql_num_fields(res); ++queryRet) *1/ */
/*             /1* { *1/ */
/*             /1*     printf("%8s ", row[queryRet]); *1/ */
/*             /1* } *1/ */
/*             printf("\n"); */
/*             while(NULL != (row = mysql_fetch_row(res))) */
/*             { */
/*                 for(queryRet = 0; (int)queryRet != (int)mysql_num_fields(res); ++queryRet){ */
/*                     printf("%8s ", row[queryRet]); */
/*                     strcpy(retStr, row[queryRet]); */
/*                 } */
/*                 printf("\n"); */
/*             } */
/*         } */
/*         mysql_free_result(res); */
/*     } */
/*     return retStr; */
/* } */
//log写入数据库
int logIntoMysql(MYSQL * conn,int  userID,char *op){
    char query[512] = { 0 };
    sprintf(query, "insert into LOG_OPERATION (userID, operation) values(%d,'%s')", userID, op);
    mysqlInsert(conn, query);
    return 0;
}

int mysqlClose(MYSQL* conn){
    mysql_close(conn); 
    return 0;
}
