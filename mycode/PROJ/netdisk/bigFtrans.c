#include"func.h"
//filesize为对端发送的文件大小;FD:对端套接字; rfd:本地文件描述符
void bigFtrans(int filesize,int FD,int rfd,int offset){
    //先判断断点位置(offset),断点在文件末尾则退出该函数
    if(offset==filesize){
        printf("tans_finished!\n");
        return (void)0;
    }
    if(filesize>=(100*1024*1024)){
        long sendlen=4096;//每次splice的大小,即管道大小
        send(FD,&sendlen,sizeof(sendlen),0);//将每次传送的大小发送给对端
        int fds[2];
        pipe(fds);
        int remain=filesize;
        while(sendlen){
            splice(rfd,NULL,fds[1],NULL,sendlen,0);
            int ret =splice(fds[0],NULL,FD,NULL,sendlen,0);
            //ERROR_CHECK(ret ,-1,"splice");
            remain-=sendlen;
            if(sendlen>remain){
                sendlen=remain;
            }            
        }
    }else{
        //断点续传
    }
}
