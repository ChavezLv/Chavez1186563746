#include"database.h"
#include<stdio.h>
#include<string.h>
MYSQL* mysqlConnect(){
    MYSQL* conn;
    char *sever="localhost";
    char *user="root";
    char *password="1234";
    char *database="myDB";
    conn =mysql_init(NULL);
    if(!mysql_real_connect(conn,sever,user,password,database,0,NULL,0)){
        printf("erro:%s\n",mysql_error(conn));
        return (MYSQL*)EXIT_FAILURE;
    }
    return conn;
}

int mysqlInsert(MYSQL* conn, char* command) {
    puts(command);
    int ret = mysql_query(conn, command);
    if (ret) {
        printf("mysql Error:%s\n", mysql_error(conn));
        return -1;
    }
    else {
    }
    return 0;
}


int mysqlSearch(MYSQL* conn, char* command, void* result)
{
    MYSQL_RES *res;
    MYSQL_ROW row;//char*类型的字符串数组
    int queryret = mysql_query(conn, command);
    if(queryret)
    {
        printf("?mysql>>Error query: %s\n", mysql_error(conn));
        strcpy((char*)result, mysql_error(conn));
        return -1;
    }
    else
    {
        res = mysql_store_result(conn);
        row = mysql_fetch_row(res);
        
        if(NULL == row){
            strcpy((char*)result, "0");
            printf("mysql:can`t search any data\n");
            return -1;
        }else{
            do
            {
                for(queryret = 0; queryret != (int)mysql_num_fields(res); ++queryret)
                {
                    strcpy((char*)result, row[queryret]);
                }
            }while(row = mysql_fetch_row(res));
        }
        //必须释放结果集
        mysql_free_result(res);
    }
    return 0;
}
