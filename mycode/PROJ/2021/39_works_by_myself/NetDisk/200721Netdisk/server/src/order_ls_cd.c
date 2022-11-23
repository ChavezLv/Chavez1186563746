#include"../include/orders.h"

int order_ls(pReOrder_t reOrder,char *orderTarget,int userId){
    //参数1：返回数据信息
    //参数2：文件路径
    //参数3：用户id

    //1.调用路径解析函数获得目录id（无参数不调用，直接是pwd）
    int dirId;
    if (!orderTarget){
        dirId = read_Addr(".",userId); 
    }
    else{
        dirId = read_Addr(orderTarget, userId);
    }
    if(-1 == dirId){        //文件路径非法
        reOrder->retNum = -1;
        strcpy(reOrder->retInfo,"Error path,please try again");
        return -1;
    }

    MYSQL *mysqlConn = mysqlConnect();

    if(!mysqlConn){
        printf("mysql init fail\n");
        reOrder->retNum = -1;
        strcpy(reOrder->retInfo,"mysql init fail");
        return -1;
    }

    //2.查找目录表和用户文件表中所有父目录id为该目录id的项
    
    //目录名
    MYSQL_RES *res;
    MYSQL_ROW row;
    char query1[200] = "select dirName from DIR_INFO where userID = ";
    sprintf(query1,"%s%d%s%d",query1,userId," and parentDirID = ",dirId);
printf(">mysql>>%s\n", query1);

    int queryRet1 = mysql_query(mysqlConn,query1);
    if(queryRet1){
printf(">mysql>>Error query:%s\n",mysql_error(mysqlConn));
        reOrder->retNum = -1;       
        strcpy(reOrder->retInfo,"Error query");
        return -1;
    }
    else{
        //返回目录名
        reOrder->retNum = 0;        
        res = mysql_store_result(mysqlConn);
        row = mysql_fetch_row(res);
        //3.返回结果给服务端，发送数据给客户端
        if(NULL == row){
            //未查询到结果
printf("Don't query any dir data\n");
            strcpy(reOrder->retInfo,"0");
        }
        else{
            //查询到目录文件名，并返回给客户端
            do{
                for(queryRet1 = 0; queryRet1 != (int)mysql_num_fields(res); ++queryRet1){
/* printf("row is %d\n", (int)mysql_num_fields(res)); */
                    strcat(&reOrder->retInfo[queryRet1], row[queryRet1]);

                    //区分目录名和文件名
                    strcat(&reOrder->retInfo[queryRet1],"* ");
                }
            }while(NULL != (row = mysql_fetch_row(res)));
        }
        mysql_free_result(res);
    }

    //文件名
    char query2[200] = {0};
    sprintf(query2, "select fileName from USER_FILE where userID = %d and dirID = %d", userId, dirId);
printf(">mysql>>%s\n", query2);
    
    int queryRet2 = mysql_query(mysqlConn,query2);
    if(queryRet2){
printf("Error query:%s\n",mysql_error(mysqlConn));
        reOrder->retNum = -1;       
        strcpy(reOrder->retInfo,"Error query");
        return -1;
    }
    else{
        //返回文件名
        reOrder->retNum = 0;        
        res = mysql_store_result(mysqlConn);
printf(">mysql>>mysql_num_rows:%lu\n",(unsigned long)mysql_affected_rows(mysqlConn));
        row = mysql_fetch_row(res);

        //3.返回结果给服务端，发送数据给客户端
        if(NULL == row){
            //未查询到结果
printf("Don't query any file data\n");
            return 0;
        }
        else{
            //查询到数据，并返回给客户端
            do{
                for(queryRet2 = 0; queryRet2 != (int)mysql_num_fields(res); ++queryRet2){
                    strcat(&reOrder->retInfo[queryRet1], row[queryRet2]);
                    queryRet1++;
                }
            }while(NULL != (row = mysql_fetch_row(res)));
        }
        mysql_free_result(res);
    }
    mysql_close(mysqlConn);
    char op[1024] = {0};
    sprintf(op, "ls %s", orderTarget);
    logIntoMysql(userId, op);
    return 0;
}

int order_cd(pReOrder_t reOrder, char *orderTarget, int userId){//reOrder指针，命令参数，发命令的userId
    int dirId = read_Addr(orderTarget, userId);
    memset(&reOrder->retInfo, 0, sizeof(reOrder->retInfo));
    
    if(dirId == -1){
        reOrder->retNum = -1;
        strcpy(reOrder->retInfo, "not this path");
        return -1;
    }

    char query[200] = {0};
    sprintf(query, "update USER_INFO set pwdDirID = %d where userID = %d", dirId, userId);
    int ret = 0;
    ret = mysql_update(query);
    if(ret == -1){
        reOrder->retNum = -1;
        strcpy(reOrder->retInfo, "get defeat");
        return -1;
    }
    strcpy(reOrder->retInfo, "success");
printf("cd to dirId %s\n", reOrder->retInfo);
    char op[1024] = {0};
    sprintf(op, "cd %s", orderTarget);
    logIntoMysql(userId, op);
    return dirId;
}

