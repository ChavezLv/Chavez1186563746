#include"process_pool.h"
#define FILENAME "file"
int transferFile(int peerfd){
    int rfd=open(FILENAME,O_RDWR);
    ERROR_CHECK(rfd, -1, "open");
    char filebuff[100]={0};
    //从文件读入用户缓冲区
	int ret = read(rfd, filebuff, sizeof(filebuff));
	printf("ret: %d\n", ret);
    //先发送文件名
	int len = strlen(FILENAME);
    //以四字节的大小发送文件名实际长度
	send(peerfd, &len, 4, 0);
    //发送文件名
	send(peerfd, FILENAME, 4, 0);

    //发送内容长度
    len=strlen(filebuff);
    send(peerfd,&len,4,0);
    //发送文件内容
    int sret=send(peerfd,&filebuff,len,0);
    return sret;
}
