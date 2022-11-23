#include"../include/orders.h"
int order_mkdir(pReOrder_t pReOrder, pWork_t pWork)
{
    pOrder_t pOrder = (pOrder_t)pWork->workArgs;
    char *orderTarget = pOrder->orderTarget;
    int *Fd = pWork->Fd;
    int clientFd = pWork->clientFd;
    int userId = Fd[clientFd];
	
    int len, len1;
	char buf[200] = { 0 };
	char *dirName;
	int ret = 0;
    int ans = 0;
    strcpy(buf, orderTarget);
	for (len = strlen(buf) - 1, len1 = len; buf[len] != '/' && len > 0; len--){}
    if(len != 0){
        buf[len] = 0;
        dirName = &buf[len + 1];
        ans = read_Addr(buf, userId);
    }
    else{
        dirName = buf;
        ans = read_Addr(".", userId);
    }
	if (ans == -1)
	{
		pReOrder->retNum = -1;
		strcpy(pReOrder->retInfo, "the path is wrong");
		return -1;
	}
	char query[200] = { 0 };
	char reStr[1000] = { 0 };
	MYSQL *conn = mysqlConnect();
	sprintf(query, "%s%d%s%s%s", "select dirID from DIR_INFO where parentDirID = ", ans, " and dirName='", dirName, "'");
	ret = mysqlSearch(conn, query, reStr);
	if (ret != -1)
	{
		pReOrder->retNum = -1;
		strcpy(pReOrder->retInfo, "the dir has been existing");
		return -1;
	}
	memset(query, 0, sizeof(query));
	sprintf(query, "%s%d%s%s%s%d%s", "insert into DIR_INFO (parentDirID,dirName,userID)values(", ans, ",'", dirName, "',", userId, ")");
	ret = mysqlInsert(conn, query);
    if(ret == -1){
        strcpy(pReOrder->retInfo, "mkdir fail");
        return -1;
    }
	mysql_close(conn);
    char op[1024] = {0};
    sprintf(op, "mkdir %s", orderTarget);
    logIntoMysql(userId, op);
	return 0;
}

int order_rmdir(pReOrder_t pReOrder, pWork_t pWork)
{
    pOrder_t pOrder = (pOrder_t)pWork->workArgs;
    char *orderTarget = pOrder->orderTarget;
    int *Fd = pWork->Fd;
    int clientFd = pWork->clientFd;
    int userId = Fd[clientFd];
	
    int ans = read_Addr(orderTarget, userId);
	if (ans == -1)
	{
		pReOrder->retNum = -1;
		strcpy(pReOrder->retInfo, "the path you send is wrong");
		return -1;
	}
	char query[200] = { 0 };
	char reStr[1000] = { 0 };
	MYSQL *conn = mysqlConnect();
	strcpy(query, "select fileID from USER_FILE where dirID=");
	sprintf(query, "%s%d", query, ans);
	int ret = 0;
	ret = mysqlSearch(conn, query, reStr);
	if (ret != -1)
	{
		pReOrder->retNum = -1;
		sprintf(pReOrder->retInfo, "%s", "it can not be deleted because it has child file");
		return -1;
	}
	memset(&query, 0, sizeof(query));
	memset(&reStr, 0, sizeof(reStr));
	strcpy(query, "select dirID from DIR_INFO where parentDirID=");
	sprintf(query, "%s%d", query, ans);
	ret = mysqlSearch(conn, query, reStr);
	if (ret != -1)
	{
		pReOrder->retNum = -1;
		sprintf(pReOrder->retInfo, "%s", "it can not be deleted because it has child dir");
		return -1;
	}
	memset(&query, 0, sizeof(query));
	memset(&reStr, 0, sizeof(reStr));
	strcpy(query, "select userID from USER_INFO where pwdDirID=");
	sprintf(query, "%s%d", query, ans);
	ret = mysqlSearch(conn, query, reStr);
	if (ret != -1)
	{
		pReOrder->retNum = -1;
		strcpy(pReOrder->retInfo, "it can not be deleted because you are visiting it");
		return -1;
	}
	memset(&query, 0, sizeof(query));
	strcpy(query, "delete from DIR_INFO where dirID=");
	sprintf(query, "%s%d", query, ans);
	ret = mysql_delete(query);
	if (ret == -1)
	{
		pReOrder->retNum = -1;
		strcpy(pReOrder->retInfo, "it cannot be delete");
		ret = -1;
	}
	pReOrder->retNum = -1;
	strcpy(pReOrder->retInfo, "delete success");
    char op[1024] = {0};
    sprintf(op, "rmdir %s", orderTarget);
    logIntoMysql(userId, op);
	return 0;
}
