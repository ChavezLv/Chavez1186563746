#include"configuration.hpp"
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/epoll.h>
#include<string.h>
#include<iostream>
#include<vector>
#include"../../../include/json/json.h"
using std::vector;
using std::endl;
using std::cout;
int recvCircle(int sfd,void *buf,int recvlen){
	int total = 0,ret;
	char *p = (char*)buf;
	while(total<recvlen){
		ret = recv(sfd,p+total,recvlen-total,0);
		if(!ret){
			return 0;
		}
		total+=ret;
	}
	return total;
}
bool _isClosed(int fd){
	int ret;
	char buf[128] = {0};
	do{
		ret = recv(fd,buf,sizeof(buf),MSG_PEEK);
	}while(ret == -1 && errno == EINTR);
	return ret == 0;
}
int epoll_add(int epfd,int fd,int state){
	struct epoll_event ev;
	ev.data.fd = fd;
	ev.events = state;
	int ret = epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&ev);
	return ret;
}
int epoll_del(int epfd,int fd,int state){
	struct epoll_event ev;
	ev.data.fd = fd;
	ev.events = state;
	int ret = epoll_ctl(epfd,EPOLL_CTL_DEL,fd,&ev);
	return ret;
}
int main(){
	rxl::Configuration conf("../../../conf/config_online");
	int sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(-1 == sockfd){perror("socket");}
	struct sockaddr_in addr;
	bzero(&addr,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(conf["port"].c_str()));
	addr.sin_addr.s_addr = inet_addr(conf["ip"].c_str());
	connect(sockfd,(struct sockaddr*)&addr,sizeof(addr));
	int epfd = epoll_create1(0);
	if(epfd < 0){
		perror("epoll_create1");
		return -1;
	}
	epoll_add(epfd,sockfd,EPOLLIN);
	epoll_add(epfd,STDIN_FILENO,EPOLLIN);
	vector<epoll_event> evtlist(2);
	while(1){
		int nready;
		do{
			nready= epoll_wait(epfd,&evtlist[0],sockfd,5000);
		}while(nready == -1 && errno == EINTR);
		if(nready == -1){
			perror("epoll_wait");
			return -1;
		}else if(nready == 0){
			continue;
		}else{
			for(int i = 0;i<nready;++i){		
				if(evtlist[i].data.fd == sockfd){
					int msglen;
					int len;
					int ret = recvCircle(sockfd,&len,sizeof(int));
					if(!ret){
						cout<<"bye bye"<<endl;
						return 0;
					}
					ret = recvCircle(sockfd,&msglen,len);
					if(!ret){
						cout<<"bye bye"<<endl;
						return 0;
					}
					char buf[65536] = {0};
					int recvlen = 0;
					while(recvlen < msglen){
						ret = recvCircle(sockfd,&len,sizeof(int));
						if(!ret){
							cout<<"bye bye"<<endl;
							return 0;
						}
						ret = recvCircle(sockfd,buf+recvlen,len);
						if(!ret){
							cout<<"bye bye"<<endl;
							return 0;
						}
						recvlen+=len;
					}	
					if(!strcmp(buf,"welcome to server!")){
						cout<<buf<<endl;
						continue;
					}
					Json::Reader reader;
					Json::Value root;
					cout<<buf<<endl;
					if(!reader.parse(buf, root, false)){
						perror("reader.parse");
						continue;
					}
					int rsize = root.size();
					cout<<"---------查询结果----------"<<endl;
					for(int i = 0; i< rsize;++i){
						cout<<"候选词:"<<root[i]["word"];
						cout<<"使用频率:"<<root[i]["fre"];
						cout<<"编辑距离:"<<root[i]["dis"];
					}
				}else{
					char buf[1024] = {0};
					read(STDIN_FILENO,buf,sizeof(buf));
					int ret = send(sockfd,buf,strlen(buf),0);
					if(ret == -1){
						cout<<"bye bye"<<endl;
						break;
					}
				}
			}
		}
	}
}
