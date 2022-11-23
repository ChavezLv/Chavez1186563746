#include"../include/orders.h"

int order_ls(pReOrder_t reOrder,pWork_t pWork){
    //
    pOrder_t pOrder = (pOrder_t)pWork->workArgs;
    char *orderTarget = pOrder->orderTarget;
    int *Fd = pWork->Fd;
    int clientFd = pWork->clientFd;
    int userId = Fd[clientFd];
    
    int nums = 0;
    int dirId;
    dirId = read_Addr(orderTarget, userId);
    if(-1 == dirId){        //文件路径非法
        reOrder->retNum = -1;
        strcpy(reOrder->retInfo,"Error path,please try again");
        return -1;
    }

    MYSQL *mysqlConn = mysqlConnect();

    MYSQL_RES *res;
    MYSQL_ROW row;
    char query1[200] = "select dirName from DIR_INFO where userID = ";
    sprintf(query1,"%s%d%s%d",query1,userId," and parentDirID = ",dirId);

    int queryRet1 = mysql_query(mysqlConn,query1);
    if(queryRet1){
        printf("?mysql>>Error query:%s\n",mysql_error(mysqlConn));
        reOrder->retNum = -1;       
        strcpy(reOrder->retInfo, "fail");
        return -1;
    }
    else{
        //返回目录名
        reOrder->retNum = 0;        
        res = mysql_store_result(mysqlConn);
        row = mysql_fetch_row(res);
        if(NULL == row){
        }
        else{
            do{
                for(queryRet1 = 0; queryRet1 != (int)mysql_num_fields(res); ++queryRet1){
                    strcat(&reOrder->retInfo[queryRet1], row[queryRet1]);
                    strcat(&reOrder->retInfo[queryRet1],"*  ");
                    ++nums;
                    if(nums >= 6){
                        strcat(&reOrder->retInfo[queryRet1],"\n");
                        //把第二个参数的字符串追加到第一个字段的结尾;
                        nums = 0;
                    }
                }
            }while(NULL != (row = mysql_fetch_row(res)));
        }
        mysql_free_result(res);
    }

    char query2[200] = {0};
    sprintf(query2, "select fileName from USER_FILE where userID = %d and dirID = %d", userId, dirId);
    
    int queryRet2 = mysql_query(mysqlConn,query2);
    if(queryRet2){
        printf("?mysql>>Error query:%s\n",mysql_error(mysqlConn));
        reOrder->retNum = -1;       
        strcpy(reOrder->retInfo, "fail");
        return -1;
    }
    else{
        reOrder->retNum = 0;        
        res = mysql_store_result(mysqlConn);
        row = mysql_fetch_row(res);

        if(NULL == row){
            return 0;
        }
        else{
            do{
                for(queryRet2 = 0; queryRet2 != (int)mysql_num_fields(res); ++queryRet2){
                    strcat(&reOrder->retInfo[queryRet1], row[queryRet2]);
                    queryRet1++;
                    strcat(&reOrder->retInfo[queryRet1],"  ");
                    ++nums;
                    if(nums >= 6){
                        strcat(&reOrder->retInfo[queryRet1],"\n");
                        nums = 0;
                    }
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

int order_cd(pReOrder_t reOrder, pWork_t pWork){//reOrder指针，命令参数，发命令的userId
    pOrder_t pOrder = (pOrder_t)pWork->workArgs;
    char *orderTarget = pOrder->orderTarget;//命令参数,此处是path
    int *Fd = pWork->Fd;
    int clientFd = pWork->clientFd;
    int userId = Fd[clientFd];
    
    
    int dirId = read_Addr(orderTarget, userId);
    memset(&reOrder->retInfo, 0, sizeof(reOrder->retInfo));

   //目录不存在 
    if(dirId == -1){
        reOrder->retNum = -1;
        strcpy(reOrder->retInfo, "not this path");
        return -1;
    }

    char query[200] = {0};
    sprintf(query, "update USER_INFO set pwdDirID = %d where userID = %d", dirId, userId);
    //把pwdDirID更改为目标文件的id号
    int ret = 0;
    ret = mysql_update(query);
    if(ret == -1){
        reOrder->retNum = -1;
        strcpy(reOrder->retInfo, "fail");
        return -1;
    }
    strcpy(reOrder->retInfo, "success");
    

    char op[1024] = {0};
    sprintf(op, "cd %s", orderTarget);
    logIntoMysql(userId, op);
    return dirId;
}

