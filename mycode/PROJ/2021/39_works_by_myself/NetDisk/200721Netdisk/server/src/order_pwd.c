#include "../include/orders.h"

int order_pwd(pReOrder_t reOrder, int userId)
{
    char comman[COMMAN_LEN] = { 0 };
    int userRootDirID = 0;  //用户家目录ID
    int pwdDirID = 0;       //用户当前工作目录ID

    char ret[512] = { 0 };  //数据库查询结果
    int searchRet = 0;      //数据库查询返回值，查询失败返回-1

    char eachPath[PATH_NUM][PATH_LEN];         //存储每个路径
    memset(eachPath, 0, PATH_LEN * PATH_NUM);


    MYSQL *conn = mysqlConnect();

    //查找用户家目录ID，存储在userRootDirID中
    sprintf(comman, 
            "select dirID from DIR_INFO where userID = %d and parentDirID = %d", userId, 0);
//puts(comman);
    searchRet = mysqlSearch(conn, comman, ret);        
    if(searchRet == -1){
        return -1;
    }
    userRootDirID = atoi(ret);

    //查找用户当前目录ID，存储在pwdDirID中
    bzero(comman, COMMAN_LEN);
    sprintf(comman, 
            "select pwdDirID from USER_INFO where userID = %d", userId);
//puts(comman);
    searchRet = mysqlSearch(conn, comman, ret);        
    if(searchRet == -1){
        return -1;
    }
    pwdDirID = atoi(ret);

    int count = 0;                           //目录个数
    int currentID = pwdDirID;                //当前目录ID

    while(1){
        sprintf(comman, 
                "select dirName from DIR_INFO where userID = %d and dirID = %d", userId, currentID);
        searchRet = mysqlSearch(conn, comman, ret);
//puts(comman);        
        if(searchRet == -1){
            return -1;
        }
        strcpy(eachPath[count], ret);
        ++count;

        bzero(comman, COMMAN_LEN);
                sprintf(comman, 
                "select parentDirID from DIR_INFO where userID = %d and dirID = %d", userId, currentID);
        searchRet = mysqlSearch(conn, comman, ret);
//puts(comman);        
        if(searchRet == -1){
            return -1;
        }
        currentID = atoi(ret);
        if(currentID == userRootDirID){
            break;
        }
    }

    char currentInfo[DIR_NAME_LEN] = { 0 };

    //拼接路径
    for(int i = count-1; i >= 0; --i){
        sprintf(currentInfo, "/%s", eachPath[i]);
        strcat(reOrder->retInfo, currentInfo);
    }
    return 0;
}
