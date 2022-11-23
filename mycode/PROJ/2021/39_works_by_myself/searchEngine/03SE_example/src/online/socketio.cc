#include"socketio.hpp"
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
using std::endl;
using std::cout;
namespace rxl{
	int SocketIO::readn(char *buf,int len){	
		int left = len;
		char *p = buf;
		int ret;
		while(left>0){
			ret = ::read(_fd,p,left);
			if(ret == -1 && errno == EINTR){
				continue;
			}else if(ret == -1){
				perror("SOCKETIO read");
				return len - left;
			}else if(ret == 0){
				return len - left;
			}else{
				left-=ret;
				p+=ret;
			}
		}
		return len - left;
	}
	int SocketIO::recvPeek(char *buf,int len){
		int ret;
		do{
			ret = ::recv(_fd,buf,len,MSG_PEEK);
		}while(ret == -1 && errno == EINTR);
		return ret;
	}
	int SocketIO::readline(char *buf,int maxlen){
		int left = maxlen -1;
		char *p = buf;
		int ret;
		int total = 0;
		while(left > 0) {
			ret = recvPeek(p, left);		
			for(int idx = 0; idx != ret; ++idx) {
				if(p[idx] == '\n') {
					int sz = idx + 1;
					readn(p, sz);
					total += sz;
					p += sz;
					*p = '\0';
					return total;
				}
			}
			readn(p, ret);
			left -= ret;
			p += ret;
			total += ret;
		}
		*p = '\0';
		return total;
	}
	int SocketIO::writen(const char* buf,int len){
		int ret;
		int left = len;
		const char* p = buf;
		while(left){
			ret = ::write(_fd,buf,left);
			if(ret == -1 && errno == EINTR){
				continue;
			}else if(ret == -1){
				perror("SOCKETIO write");
				return len - left;
			}else{
				left-=ret;
				p+=ret;
			}
		}
		return len - left;
	}
	typedef struct{	
		int len;
		char buf[1000];
	}train_t;
	int SocketIO::writen_train(const char* jsonbuf,int len){
		int countsend = 0;
		train_t train;
		train.len = sizeof(int);
		memcpy(train.buf,&len,train.len);
		::send(_fd,&train,train.len+4,0);
		while(countsend != len){
			bzero(&train,sizeof(train));
			int sendsize = sizeof(train.buf)>(len-countsend)?(len-countsend):sizeof(train.buf);
			memcpy(train.buf,jsonbuf+countsend,sendsize);	
			train.len = sendsize;
			int ret = send(_fd,&train,train.len+4,0);
			if(ret == -1){
				return -1;
			}
			countsend +=train.len;
		}
		return countsend;
	}
}//end of namespace
