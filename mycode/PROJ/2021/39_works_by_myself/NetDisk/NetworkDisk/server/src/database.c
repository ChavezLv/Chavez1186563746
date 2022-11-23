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
        printf("Error connecting to database: %s\n", mysql_error(mysqlConn));
        return NULL;
    }
    return mysqlConn;
}

int mysqlInsert(MYSQL* conn, char* command) {
    puts(command);
    int queryResult = mysql_query(conn, command);
    if (queryResult) {
        printf("Error making query:%s\n", mysql_error(conn));
    }
    else {
        printf("insert success\n");
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
        printf("Error query: %s\n", mysql_error(conn));
        return -1;
    }
    else
    {
        res = mysql_store_result(conn);
        /* printf("mysql_affacted_rows: %lu\n", (unsigned long)mysql_affected_rows(conn)); */
        printf("mysql_num_rows: %lu\n", (unsigned long)mysql_num_rows(res));

        row = mysql_fetch_row(res);
        if (NULL == row)
        {
            printf("Don't query any data\n");

        }
        else
        {
            do
            {
                printf("num row: %lu\n", (unsigned long)mysql_num_rows(res));
                printf("num fileds: %lu\n", (unsigned long)mysql_num_fields(res));
                for (queryRet = 0; queryRet != (int)mysql_num_fields(res); ++queryRet)
                {
                    printf("%8s ", row[queryRet]);
                    userID = atoi(row[queryRet]);
                }
                printf("\n");
            } while (NULL != (row = mysql_fetch_row(res)));
        }
        mysql_free_result(res);
    }
    return userID;
}
//返回根据查询字段得到的结果
char* mysqlSearch(MYSQL* conn, char* command) {
    MYSQL_RES* res;
    MYSQL_ROW row;
    char* retStr = (char*)calloc(1, 300);
    unsigned int queryRet;
    queryRet = mysql_query(conn, command);
    if (queryRet)
    {
        printf("Error query: %s\n", mysql_error(conn));
        return NULL;
    }
    else
    {
        res = mysql_store_result(conn);
        /* printf("mysql_affacted_rows: %lu\n", (unsigned long)mysql_affected_rows(conn)); */
        printf("mysql_num_rows: %lu\n", (unsigned long)mysql_num_rows(res));

        row = mysql_fetch_row(res);
        if (NULL == row)
        {
            /* printf("query empty data\n"); */
            printf("empty set\n");
            return NULL;
        }
        else {
            /* for(queryRet = 0; queryRet != (int)mysql_num_fields(res); ++queryRet) */
            /* { */
            /*     printf("%8s ", row[queryRet]); */
            /* } */
            printf("\n");
            while (NULL != (row = mysql_fetch_row(res)))
            {
                for (queryRet = 0; (int)queryRet != (int)mysql_num_fields(res); ++queryRet) {
                    printf("%8s ", row[queryRet]);
                    strcpy(retStr, row[queryRet]);
                }
                printf("\n");
            }
        }
        mysql_free_result(res);
    }
    return retStr;
}
int mysqlClose(MYSQL* conn) {
    mysql_close(conn);
    return 0;
}
