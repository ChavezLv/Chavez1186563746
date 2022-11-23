#include <mysql/mysql.h>
#include<stdio.h>
int main(void){
    MYSQL * conn =NULL;
    char *host ="localhost";
    char *user ="root ";
    char *passwd="Ll!159370";
    char *db="STU";
    //初始化mysql的连接句柄
    conn =mysql_init(NULL);
    //建立连接
    if(mysql_real_connect(conn,host,user,passwd,db,0,NULL,0)==NULL){
        printf("error:%s\n",mysql_error(conn));
        return EXIT_FAILURE;
    }

    //执行查询
    char *query="select *from score";
    int ret =mysql_query(conn,query);
    if(ret!=0){
        printf("error query1:%s\n",mysql_error(conn));
        return EXIT_FAILURE;
    }
    //获取结果集,N行数据
    MYSQL_RES* result=mysql_store_result(conn);//把所有查询结果存储下来
    if(result==NULL){
        printf("error query2:%s\n",mysql_error(conn));
        return EXIT_FAILURE;
    }
    int rows=mysql_num_rows(result);//行数
    int cols=mysql_num_fields(result);//列数
    printf("rows:%d,cols:%d\n",rows,cols);
    //获取每一行数据
    MYSQL_ROW row ;
    while((row=mysql_fetch_row(result))!=NULL){
        for(int i=0;i<cols;i++){
            printf("%10s",row[i]);
        }
        printf("\n");
    }
    mysql_free_result(result);
    mysql_close(conn);
    return 0;
}
