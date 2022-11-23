#include"SockerIO.h"
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
#include<errno.h>
SockerIO::SockerIO(int fd):_fd(fd){}
SockerIO::~SockerIO(){
    close(_fd);
}
int SockerIO::readn(char *pstr,int len){
    int leave=len;
    int ret=0;
    while(leave>0){
        ret=read(_fd,pstr,leave);
        if(-1==ret&&errno==EINTR){
            continue;
        }
        else if (-1==ret){
            perror("read error -1");
            return len-ret;
        }
        else if(0==ret){
            break;//EOF
        }
        else{
            pstr+=ret;
            leave-=ret;//预防出现一次无法读完的情况
        }
    }
}
int SockerIO::writen(const char *pstr,int len){
 int leave = len;
    int ret = 0;

    while(leave > 0)
    {
        ret = write(_fd, pstr, leave);
        if(-1 == ret && errno == EINTR)
        {
            continue;
        }
        else if(-1 == ret)
        {
            perror("writen error -1");
            return len - ret;
        }
        else if(0 == ret)
        {
            break;
        }
        else
        {
            pstr += ret;
            leave -= ret;
        }
    }
    return len - leave;//全部写入的情况返回值大小就是len
}
int SockerIO::readLine(char *pstr,int len){
    int leave=len-1;
    int ret=0,total=0;
    while(leave>0){
        ret=recv(_fd,pstr,leave,MSG_PEEK);
        if(-1 == ret && errno == EINTR)
        {
            continue;
        }
        else if(-1 == ret)
        {
            perror("readLine error -1");
            return len - ret;
        }
        else if(0 == ret)
        {
            break;
        }
        else{
            for(int idx=0;idx<ret;++idx){
                if(pstr[idx]=='\n'){
                    int sz=idx+1;
                    readn(pstr,sz);
                    pstr+=sz;
                    *pstr='\0';
                    return total+sz;
                }
            }
            readn(pstr,ret);
            total+=ret;
            pstr+=ret;
            leave-=ret;
        }
    }
    *pstr='\0';
    return total-leave;
}
