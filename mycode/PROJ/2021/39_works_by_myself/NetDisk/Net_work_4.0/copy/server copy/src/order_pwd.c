#include "../include/orders.h"

int order_pwd(pReOrder_t reOrder, pWork_t pWork)
{
    int *Fd = pWork->Fd;
    int clientFd = pWork->clientFd;
    int userId = Fd[clientFd];
    
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
    searchRet = mysqlSearch(conn, comman, ret);        
    if(searchRet == -1){
        return -1;
    }
    userRootDirID = atoi(ret);

    bzero(comman, COMMAN_LEN);
    sprintf(comman, 
            "select pwdDirID from USER_INFO where userID = %d", userId);
    searchRet = mysqlSearch(conn, comman, ret);        
    if(searchRet == -1){
        return -1;
    }
    pwdDirID = atoi(ret);
    if(pwdDirID == userRootDirID){
        strcpy(reOrder->retInfo, "/");
        return 0;
    }

    int count = 0;                           //目录个数
    int currentID = pwdDirID;                //当前目录ID

    while(1){
        sprintf(comman, 
                "select dirName from DIR_INFO where userID = %d and dirID = %d", userId, currentID);
        searchRet = mysqlSearch(conn, comman, ret);
        if(searchRet == -1){
            return -1;
        }
        strcpy(eachPath[count], ret);
        ++count;

        bzero(comman, COMMAN_LEN);
                sprintf(comman, 
                "select parentDirID from DIR_INFO where userID = %d and dirID = %d", userId, currentID);
        searchRet = mysqlSearch(conn, comman, ret);
        if(searchRet == -1){
            return -1;
        }
        currentID = atoi(ret);
        if(currentID == userRootDirID){
            break;
        }
    }

    char currentInfo[DIR_NAME_LEN] = { 0 };

    for(int i = count-1; i >= 0; --i){
        sprintf(currentInfo, "/%s", eachPath[i]);
        strcat(reOrder->retInfo, currentInfo);
    }
    
    char op[1024] = {0};
    sprintf(op, "pwd");
    logIntoMysql(userId, op);
    return 0;
}



//未采用
int order_mv(pReOrder_t pReOrder, pWork_t pWork)
{
    pOrder_t pOrder = (pOrder_t)pWork->workArgs;
    char *orderTarget = pOrder->orderTarget;
    int *Fd = pWork->Fd;
    int clientFd = pWork->clientFd;
    int userId = Fd[clientFd];
	
    char addr1[200] = { 0 };//原始路径包含目录和文件名
	char addr2[200] = { 0 };//目标路径
	int ans1 = 0, ans2 = 0;
	sscanf(orderTarget, "%s %s", addr1, addr2);
	
    char *fileName;
	int len = strlen(addr1) - 1;
	for (; addr1[len] != '/'&&len > 0; len--) {}
	if (len != 0)
	{
		addr1[len] = 0;
		fileName = &addr1[len + 1];
		ans1 = read_Addr(addr1, userId);
	}
	else
	{
		fileName = addr1;
		ans1 = read_Addr(".", userId);
	}
	if (ans1 == -1)
	{
		pReOrder->retNum = -1;
		strcpy(pReOrder->retInfo, "the srcpath you send is wrong\n");
		return -1;
	}
	char query[200] = { 0 };
	char reStr[200] = { 0 };
	int ret = 0;
	sprintf(query, "%s%d", "select fileID from  USER_FILE where dirID= ", ans1);
	MYSQL *conn = mysqlConnect();
	ret = mysqlSearch(conn, query, reStr);
	if (ret != -1)
	{
		pReOrder->retNum = -1;
		strcpy(pReOrder->retInfo, "the file do not exist");
		return -1;
	}
	int fileId = atoi(reStr);
	ans2 = read_Addr(addr2, userId);
	if (ans2 == -1)
	{
		pReOrder->retNum = -1;
		strcpy(pReOrder->retInfo, "the destpath you send is wrong\n");
		return -1;
	}
	memset(query, 0, sizeof(query));
	memset(reStr, 0, sizeof(reStr));
	sprintf(query, "%s%d%s%d%s%d%s%s%s", "insert into USER_FILE (userID,dirID,fileId,fileName)values(", userId, ",", ans2, ",", fileId, ",", fileName, ")");
	ret = mysqlInsert(conn, query);
	if (ret == -1)
	{
		pReOrder->retNum = -1;
		strcpy(pReOrder->retInfo, "mv fail");
		return -1;
	}
	memset(query, 0, sizeof(query));
	memset(reStr, 0, sizeof(reStr));
	sprintf(query, "%s%d%s%d", "delete from USER_FILE where dirID = ", ans1, " and fileID = ", fileId);
	ret = mysql_delete(query);
	if (ret == -1)
	{
		pReOrder->retNum = -1;
		strcpy(pReOrder->retInfo, "mv fail");
		return -1;
	}
	pReOrder->retNum = 0;
	strcpy(pReOrder->retInfo, "mv success");
	return 0;
}
