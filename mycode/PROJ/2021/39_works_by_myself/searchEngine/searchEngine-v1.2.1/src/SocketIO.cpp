/*================================================================
*   
*   @file       ：SocketIO.hpp
*   @date       ：2021年08月29日 21:09:36
*   @description：
*
================================================================*/
 
#include "SocketIO.hpp"

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <iostream>

namespace wd
{

int SocketIO::readn(char * buff, int len)
{
	char * pbuf = buff;
	int left = len;
	int ret = 0;
	while(left > 0) {
		ret = ::recv(_fd, pbuf, left, 0);
		if(ret == 0) {
			break;
		} else if (ret == -1 && errno == EINTR) {
			continue;
		} else if (ret == -1) {
			perror("recv");
			/* exit(EXIT_FAILURE); */  //不能这样写
			break;
		} else {
			pbuf += ret;
			left -= ret;
		}
	}
	return len - left;
}

//1000
int SocketIO::readline(char * buff, int maxlen)
{
	char * pbuf = buff;
	int left = maxlen - 1;
	int total = 0;
	while(left > 0) {
		int ret = ::recv(_fd, pbuf, left, MSG_PEEK);
// std::cout << "SocketIO::readline, pbuf: " << pbuf << std::endl;
		if(ret == 0) {
			break;
		} else if(ret == -1 && errno ==EINTR) {
			continue;
		} else if(ret == -1) {
			break;
		} else { //ret > 0
			if(ret + total > maxlen - 1) {
				ret = maxlen - 1 - total;
			}

			for(int idx = 0; idx < ret; ++idx) {
				if(pbuf[idx] == '\n') {
					int sz = idx + 1;
					readn(pbuf, sz);
					pbuf[sz - 1] = '\0';
					return sz + total;
				}
			}
			//没有找到 \n；
			readn(pbuf, ret);
			pbuf += ret;
			left -= ret;
			total += ret;
		}
	}//end of while
	buff[maxlen - 1] = '\0';
	return maxlen - 1;
}

int SocketIO::writen(const char * buff, int len)
{
	const char * pbuf = buff;
	int left = len;
	int ret = 0;
	while(left > 0) {
		ret = ::send(_fd, pbuf, left, 0);//SIGPIPE信号 默认处理方式:程序退出
		if (ret == -1) {
			perror("send");
			break;
		} else {
			pbuf += ret;
			left -= ret;
		}
	}
	return len - left;
}


}//end of namespace wd
