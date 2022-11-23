#include "../include/orders.h"

int order_rm(pReOrder_t pReOrder, pWork_t pWork)
{
    pOrder_t pOrder = (pOrder_t)pWork->workArgs;
    char *orderTarget = pOrder->orderTarget;
    int *Fd = pWork->Fd;
    int clientFd = pWork->clientFd;
    int userId = Fd[clientFd];
	//rm的原理：找到文件然后删除
	//orderTarget里面是路径
	char addr[200] = { 0 };
	int ans = 0;
	strcpy(addr, orderTarget);
	char *fileName = addr;
	int len = strlen(addr) - 1;
	for (; addr[len] != '/'&&len > 0; len--) {}
	if (len != 0)
	{
		addr[len] = 0;
		fileName = &addr[len + 1];
		ans = read_Addr(addr, userId);
	}
	else
	{
		ans = read_Addr(".", userId);
	}
	if (ans == -1)
	{
		pReOrder->retNum = -1;
		strcpy(pReOrder->retInfo, "the path you send is wrong");
		return -1;
	}
	char query[200] = { 0 };
	char reStr[200] = { 0 };
	int ret = 0;
	//查找用户文件表
	sprintf(query, "%s%d%s%s%s", "select fileID from USER_FILE where dirID = ", ans, " and fileName= '", fileName,"'");
	MYSQL *conn = mysqlConnect();
	ret = mysqlSearch(conn, query, reStr);
	if (ret != 0)
	{
		pReOrder->retNum = -1;
		strcpy(pReOrder->retInfo, "the file do not exist");
		return -1;
	}
	int fileId = atoi(reStr);
	//删除用户文件表
	memset(query, 0, sizeof(query));
	memset(reStr, 0, sizeof(reStr));
	sprintf(query, "%s%d", "delete from USER_FILE where fileID = ", fileId);
	ret = mysql_delete(query);
	if (ret == -1)
	{
		pReOrder->retNum = -1;
		strcpy(pReOrder->retInfo, "rm fail");
		return -1;
	}
	//读取文件表引用计数
	memset(query, 0, sizeof(query));
	memset(reStr, 0, sizeof(reStr));
	sprintf(query, "%s%d", "select referCnt from FILE_INFO where fileID= ", fileId);
	ret = mysqlSearch(conn, query, reStr);
	int cnt = atoi(reStr);
	//修改文件表引用计数
	memset(query, 0, sizeof(query));
	memset(reStr, 0, sizeof(reStr));
	sprintf(query, "%s%d", "update FILE_INFO set referCnt = ",cnt-1 );
	ret = mysql_update(query);
	if (ret != -1)
	{
		pReOrder->retNum = -1;
		strcpy(pReOrder->retInfo, "something wrong happened");
		return -1;
	}
	pReOrder->retNum = 0;
	strcpy(pReOrder->retInfo, "success");
    
    char op[1024] = {0};
    sprintf(op, "rm %s", orderTarget);
    logIntoMysql(userId, op);
	return 0;
}
