#ifndef __RXL_MYSERVER_H__
#define __RXL_MYSERVER_H__
#include"task.hpp"
#include"tcpserver.hpp"
#include"threadpool.hpp"
#include"configuration.hpp"
#include"dictProducer.hpp"
#include"cacheManager.hpp"
#include"timerthread.hpp"
#include<stdlib.h>
using std::atoi;
namespace rxl{

class MyServer{
public:
	MyServer(Configuration& conf)
	:_conf(conf)
	,_tcpserver(_conf["ip"],atoi(_conf["port"].c_str()))
	,_threadpool(atoi(_conf["threadnum"].c_str()),atoi(_conf["queuenum"].c_str()))
	,_pcache(CacheManager::getInstance(_conf))
	,_timethread(atoi(_conf["initime"].c_str()),atoi(_conf["intervaltime"].c_str()),std::bind(&CacheManager::update,_pcache))
	{
		_pcache->init();
		_timethread.start();
	}
	void start(){
		_threadpool.start();
		_tcpserver.setConnectCallBack(std::bind(&MyServer::connectCallBack,this,std::placeholders::_1));
		_tcpserver.setMessageCallBack(std::bind(&MyServer::msgCallBack,this,std::placeholders::_1));
		_tcpserver.setCloseCallBack(std::bind(&MyServer::closeCallBack,this,std::placeholders::_1));
		_tcpserver.start();
	}
private:
	void connectCallBack(const rxl::TcpconnectionPtr& conn){
		cout<<"connect established"<<endl;
		cout<<conn->toString()<<endl;
		conn->send("welcome to server!");
	}

	void msgCallBack(const rxl::TcpconnectionPtr& conn){
		DictProducer* pdic = DictProducer::getInstance(_conf);
		string str = conn->receive();
		str[str.size()-1] = '\0';
		MyTask task(str,conn,pdic,_pcache);
		_threadpool.addTask(std::bind(&MyTask::response,task));
	}
	void closeCallBack(const rxl::TcpconnectionPtr& conn){
		cout<<"connect closed"<<endl;
		cout<<conn->toString()<<endl;
	}
private:
	Configuration& _conf;
	TcpServer _tcpserver;
	Threadpool _threadpool;
	CacheManager* _pcache;
	TimerThread _timethread;
};

}//end of namespace
#endif 
