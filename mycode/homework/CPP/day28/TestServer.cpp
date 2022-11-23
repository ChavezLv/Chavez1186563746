#include "eventLoop.h"
#include "ThreadPool.h"
#include "Acceptor.h"
#include "tcpConn.h"
#include "tcpServer.h"
#include <iostream>
#include <unistd.h>

using std::cout;
using std::endl;
ThreadPool* gPool=nullptr;

class MyTask{
public:
    MyTask(const string &msg,const TcpConnectionPtr con)
        :_msg(msg)
         ,_con(con){}
    void process(){
        /*将消息经过一系列处理后交由sendInLoop,此处不做操作*/
        _con->sendInLoop(_msg);
    }
    private:
    string _msg;
    TcpConnectionPtr _con;
};

void onConnection(const TcpConnectionPtr &con){
    cout<<con->toString()<<"has connected"<<endl;
}

void onMessage(const TcpConnectionPtr &con){
    string msg=con->receive();
    /*收到消息后传给MyTask*/
    cout <<"messages="<<msg<<endl;
    MyTask task(msg,con);
    /*在信息处理时,将具体执行交由线程池;*/
    gPool->addTask(std::bind(&MyTask::process,task));
}
void onClose(const TcpConnectionPtr& con){
    cout <<con->toString()<<"has closed"<<endl;
}
void test(){
    ThreadPool pool(4,10);
    pool.start();
    gPool=&pool;
    TcpServer server("127.0.0.1",8888);
    server.setAllCallback(std::move(onConnection),
                          std::move(onMessage),
                          std::move(onClose));
    server.start();
}

int main (int argc,char**argv){
    test();
    return 0;
}
