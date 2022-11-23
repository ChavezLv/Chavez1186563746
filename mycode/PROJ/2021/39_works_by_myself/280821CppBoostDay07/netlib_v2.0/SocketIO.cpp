/*================================================================
*   
*   @file       ：SocketIO.cpp
*   @date       ：2021年08月29日 22:24:07
*   @description：
*
================================================================*/


#include "SocketIO.hpp"
#include <stdio.h>
#include <errno.h>// errno != " #include <error.h> // error"
#include <sys/socket.h>
#include <sys/types.h>
/* #include <iostream> */

/* #ifndef errno */
/* extern int errno; */
/* #endif */


namespace wd{

int SocketIO::readn(char *buf, size_t len){
    char *pBuffer = buf;
    int leftover = len;
    int ret = 0;
    while(leftover > 0){
        ret = :: recv(_fd, pBuffer, leftover, 0);

        if(0 == ret){
            break;
        }else if( -1 == ret && errno == EINTR ){
            continue;
        }else if(-1 == ret){
            perror("recv");
            break;
        }else{
            pBuffer += ret;
            leftover -= ret;
        }
    }
/* std::cout << "int SocketIO::readn(char *buf, size_t len)" << std::endl; */
    return len - leftover;
}

int SocketIO::readline(char *buf, size_t maxlen){
    char *pBuffer = buf;
    int leftover = maxlen - 1;
    int total = 0;
    int ret = 0;
    while(leftover > 0){
        ret = ::recv(_fd, pBuffer, leftover, MSG_PEEK);//就看看, 不动缓冲区
        if(0 == ret){
            break;
        }else if( -1 == ret && errno == EINTR ){
            continue;
        }else if(-1 == ret){
            perror("recv");
            break;
        }else{
            if(ret + total > (int)maxlen - 1){
/* std::cout << "int SocketIO::readline(char *buf, size_t maxlen) ret: " << ret << std::endl; */
                ret = maxlen - 1 - total;//避免超过maxlen
            }

            for(int idx = 0; idx < ret; ++idx){
                if(pBuffer[idx] == '\n'){
                    int readnLen = idx + 1; 
                    int readnRet = readn(pBuffer, readnLen);//判断是否readn客户端就退出了

                    if(readnRet != readnLen){
                        //if client exist
                        pBuffer[readnRet] = '\0'; 
                        return total + readnRet;
                    }

                    pBuffer[readnLen] = '\0';
                    return readnLen + total;

                }
            }//enf of for(int idx = 0; idx < ret; ++idx){  }

            //没有\n, continue to fine '\n'
            readn(pBuffer, ret);
            pBuffer += ret;
            leftover -= ret;
            total += ret;
        }
    }//end of while(leftover > 0){}

    //已经满了, 才退出
    pBuffer[maxlen - 1] = '\0';
    return maxlen - 1;
}

int SocketIO::writen(const char *buf, size_t len){
    const char *pBuffer = buf;
    int leftover = len;
    int ret = 0;
    while(leftover > 0){
        ret = ::send(_fd, pBuffer, leftover, MSG_NOSIGNAL);
        if(-1 == ret){
            perror("send");
            break;
        }else{
            pBuffer += ret;
            leftover -= ret;
        }
    }
    return len - leftover;
}

}//end of namespace wd

