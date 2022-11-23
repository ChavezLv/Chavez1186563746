#include "orders.h"
#include "transfile.h"


int Sendfile(int clientfd,  int filesize, char *md5, int offset,char* filename)
{
    char path[1024];
    sprintf(path, "/file/%s", md5);//服务器文件位置
    puts(path);
    int fd = open(path, O_RDONLY);
    //先发送文件名
    train_t train;
    memset(&train, 0, sizeof(train));
    train.length = strlen(filename);
    strncpy(train.data, filename, train.length);
    sendCircle(clientfd, (const char *)&train, 4 + train.length);


    off_t cnt = 0;
    int ret = 0;
    //函数获取的实际文件大小,传入的filesize是虚拟文件表查询得到
    struct stat st;
    fstat(fd, &st);

    sendCircle(clientfd, &filesize, sizeof(filesize));
    //发送大文件
    //循环发送文件内容
    if (offset == filesize)
    {
        printf("tans_finished!\n");
        return 0;
    }
    if (filesize >= (100 * 1024 * 1024))
    {
        long sendlen = 4096;                    //每次splice的大小,即管道大小
        send(clientfd, &sendlen, sizeof(sendlen), 0); //将每次传送的大小发送给对端
        int fds[2];
        pipe(fds);
        int remain = filesize;
        while (sendlen)
        {
            //直接从偏移位置读取传送
            splice(fd, offset, fds[1], NULL, sendlen, 0);
            ret = splice(fds[0], NULL, clientfd, offset, sendlen, 0);
            ERROR_CHECK(ret ,-1,"splice");
            remain -= sendlen;
            if (sendlen > remain)
            {
                sendlen = remain;
            }
        }
    }
    else
    {   
        if (offset == filesize)
        {
            printf("already finished.\n");
            return 0;
        }
        lseek(fd, offset, SEEK_SET); //断点续传,lseek设置文件偏移位置
        train_t train;
        memset(&train, 0, sizeof(train));
        int cntRead = 0, cntSend = 0;
        while (1)
        {
            train.length = read(fd, train.data, sizeof(train.data));
            cntRead += train.length;
            if (0 == train.length)
            {
                break;
            }
            /* printf("train.len = %d\n",train.len); */
            ret = send(clientfd, &train, train.length + 4, MSG_NOSIGNAL);
            ERROR_CHECK(ret, -1, "send");
            cntSend += ret - 4;
        }
    }
    close(clientfd);
    return 0;
}



int recvfile(int clientfd, int filesize, char *md5, int offset, char *filename)
{   char *path;
    sprintf(path, "/files/%s", md5);
    int ret = 0;
    train_t train;
    memset(&train, 0, sizeof(train));

    printf("%s\n", path);
    int fd = open(path, O_CREAT | O_RDWR, 0666);
    ERROR_CHECK(fd, -1, "open");
    char buff[4096]={0};

    printf("%s\n", path);
    //接收操作已就绪
    send(clientfd, "yes", 3, 0);
    //文件上传过程记录
    int downloadSize = 0;
    float downloadRate;
    int writeSize = 0;
    int percent = filesize / 100;
    int splicePrint = 0;
    //预计完成时间
    time_t timeLast = 0, timeNow = 0;
    time(&timeLast);
    
	memset(buff, 0, sizeof(buff));
	//接收文件的大小
	off_t length = 0;
	ret = recvCircle(clientfd, (char*)&length, sizeof(length));
	printf("filelength: %ld\n", length);

    int cnt=0;
	int fds[2];
	pipe(fds);
     if (filesize >= (100 * 1024 * 1024)){
    //循环发送文件内容
	while(cnt < filesize) {
		ret = splice(fd, offset, fds[1], NULL, 4096, SPLICE_F_MORE);//从rfd->fds[1]
		splice(fds[0], NULL, clientfd, offset, ret, SPLICE_F_MORE);//fds[0]->peerfd
		
        //接收完毕返回
        if (0 == train.len)
        {
            close(fd);
            printf("train.len = 0\n");
            break;
        }
        //处理服务端的子进程关闭的情况
        if (0 == ret)
        {
            printf("ret over: %d\n", train.len);
            return 0;
        }
        cnt += ret;
    }
}
    //更新数据库
    char query[512] = {0};
    MYSQL *conn = mysqlConnect();
    bzero(query, sizeof(query));
    sprintf(query, "insert into FILE_INFO (filesize, md5, type) \
            values (%d, '%s', %c)",
            filesize, md5,type);
    mysqlInsert(conn, query);

    bzero(query, sizeof(query));
    sprintf(query, "select fileID from FILE_INFO where MD5 = '%s'", md5);
    char result[256];
    int fileid = mysqlSearch(conn, query,result);

    bzero(query, sizeof(query));
    sprintf(query, "insert into USER_FILE (dirID, userID, fileID, fileName) values (%d, %d, '%s')",
            fileid, userid, filename);//userid
    mysqlInsert(conn, query);
    return 0;
}
