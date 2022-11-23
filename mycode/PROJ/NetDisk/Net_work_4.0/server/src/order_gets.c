#include"orders.h"
int order_gets(pReOrder_t pReOrder, pWork_t pWork)
{   
    pOrder_t pOrder = (pOrder_t)pWork->workArgs;
    char *orderTarget = pOrder->orderTarget;
    int *Fd = pWork->Fd;
    int clientFd = pWork->clientFd;
    int userId = Fd[clientFd]; 
    
    int ans;
	char addr[200] = { 0 };//�����Ŀ¼+�ļ���
	strcpy(addr, orderTarget);
	int len;
    char *fileName = addr;
	for (len = strlen(addr) - 1; addr[len] != '/' && len > 0; len--) {}
    if(len > 0){
        addr[len] = 0;
        fileName = &addr[len + 1];
	    ans = read_Addr(addr, Fd[clientFd]);//�õ�Ŀ¼ID
    }
    else{
        ans = read_Addr(".", Fd[clientFd]);
    }
	//�����ļ��û����õ��ļ���id
	int ret = 0;
	char query[200] = { 0 };
	char reStr[128] = { 0 };//���صĽ��
	sprintf(query, "select fileID from USER_FILE where dirID= %d and\
            fileName = '%s'", ans, fileName);
	MYSQL *conn = mysqlConnect();
	ret = mysqlSearch(conn, query, reStr);
    if(ret == -1){
        pReOrder->retNum = -1;
        sprintf(pReOrder->retInfo, "%s is not exist in this path", fileName);
        return -1;
    }
    int fileID = atoi(reStr);
	//ͨ���ļ���FILE_INFO �õ��û���MD5��
	memset(query, 0, sizeof(query));
	memset(reStr, 0, sizeof(reStr));
	sprintf(query, "%s%d", "select MD5 from FILE_INFO where fileID=", fileID);
	ret = mysqlSearch(conn, query, reStr);
	if (ret == -1)
	{
        pReOrder->retNum = -1;
        sprintf(pReOrder->retInfo, "fail");
		return -1;
	}
    pWork_t pNewWork = (pWork_t)calloc(1, sizeof(Work_t));
    pOrder_t pOrder_med = (pOrder_t)calloc(1, sizeof(Order_t));
    pOrder_med->orderNum = SENDFILE;
    sprintf(pOrder_med->orderTarget, "%s %s" , reStr, fileName);

    memcpy(pNewWork->workArgs, pOrder_med, sizeof(Order_t));
    free(pOrder_med);
    pNewWork->clientFd = clientFd;
    pNewWork->Fd = Fd;
    pNewWork->pPool = pWork->pPool;
    
    pReOrder->retNum = SENDFILE;
    sprintf(pReOrder->retInfo, "%d %p", Fd[clientFd], pNewWork);
    
    char op[1024] = {0};
    sprintf(op, "gets %s", orderTarget);
    logIntoMysql(userId, op);
    return 0;
}
int newFd_isWhose(pWork_t pWork)
{
    int userId;
    pWork_t pNewWork;

    pOrder_t pOrder = (pOrder_t)pWork->workArgs;
    sscanf(pOrder->orderTarget, "%d %p", &userId, &pNewWork);
printf("$system>>userId:%d  pNewWork:%p\n", userId, pNewWork);
    if(pNewWork->Fd[pNewWork->clientFd] == userId){
        pNewWork->clientFd = pWork->clientFd; 
    }
    pPool_t pPoolOrder = (pPool_t)pWork->pPool;
    epoll_ctl(4, EPOLL_CTL_DEL, pWork->clientFd, NULL);    

    pthread_mutex_lock(&pPoolOrder->mutex);
    work_Add(pPoolOrder->Queue, pNewWork);
    pthread_mutex_unlock(&pPoolOrder->mutex);
    pthread_cond_signal(&pPoolOrder->cond);

    return 0;
}
