#include "../../include/func.h"
#include "../../include/threadPool.h"

typedef struct
{
	int dataLen;
	char buf[1000];
}Train_t;
MYSQL *mysqlConnect();
int read_Addr(char *command);
int mysqlSearch(MYSQL *conn, char *command, void *reStr);
int gets(int sfd, char *orderTarget)
{
	char addr[200] = { 0 };//存的是目录+文件名
	strcpy(addr, orderTarget);
	int len1 = strlen(addr) - 1, len2 = strlen(addr);
	for (len1; addr[len1] != '/'; len1--) {}
	addr[len1] = 0;
	int ans = read_Addr(addr);//得到目录ID
	char fileName[20] = { 0 };
	for (int i = 0; len1 < len2; len1++)
	{
		fileName[i++] = addr[len1 + 1];
	}
	//现在文件用户表得到文件的id
	int ret = 0;
	char query[200] = { 0 };
	char reStr[128] = { 0 };//返回的结果
	sprintf(query, "%s%d", "select fileID from USER_FILE where dirID=", ans);
	MYSQL *conn = mysqlConnect();
	mysqlSearch(conn, query, reStr);
	int fileID = atoi(reStr);
	//通过文件表FILE_INFO 得到用户的MD5码
	memset(query, 0, sizeof(query));
	memset(reStr, 0, sizeof(reStr));
	sprintf(query, "%s%d", "select MD5 from FILE_INFO where fileID=", fileID);
	ret = mysqlSearch(conn, query, reStr);
	if (ret == -1)
	{
		printf("该文件不存在\n");
		return -1;
	}
	int fd = open(reStr, O_RDWR);
	ERROR_CHECK(fd, -1, "open");

	struct stat fileInfo;
	memset(&fileInfo, 0, sizeof(fileInfo));
	//获取当前文件的状态
	fstat(fd, &fileInfo);

	Train_t train;
	memset(&train, 0, sizeof(train));

	train.dataLen = strlen(fileName);
	strcpy(train.buf, fileName);

	//发送文件名字
	send(clientFd, &train, 4 + train.dataLen, 0);


	return 0;
}
