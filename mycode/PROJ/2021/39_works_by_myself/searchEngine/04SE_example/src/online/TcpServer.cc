#include "../../include/TcpServer.hpp"

namespace spellCorrect
{

TcpServer::TcpServer(const string& ip,unsigned short port)
    :_acceptor(ip,port)
     ,_loop(_acceptor)
    {}

void TcpServer::setConnectionCallback(TcpConnectionCallback && cb)
{
    _loop.setConnectionCallback(std::move(cb));
}

void TcpServer::setMessageCallback(TcpConnectionCallback && cb)
{
    _loop.setMessageCallback(std::move(cb));
}

void TcpServer::setCloseCallback(TcpConnectionCallback && cb)
{
    _loop.setCloseCallback(std::move(cb));
}

void TcpServer::start()
{
    _acceptor.ready();
    _loop.loop();
}

void TcpServer::stop()
{
    _loop.unloop();
}

}//end of namespace spellCorrect

#if 0
#include "../../include/Thread.hpp"
#include "../../include/ThreadPool.hpp"
#include "../../include/TcpConnection.hpp"
#include <iostream>


using namespace spellCorrect;
using std::cout;
using std::endl;
using std::unique_ptr;

spellCorrect::ThreadPool* gPool = nullptr;

class MyTask
{
public:
    MyTask(const string& msg,const TcpConnectionPtr &conn)
    :_msg(msg)
     ,_conn(conn)
    {}

    void process()
    {
        cout << ">> " << current_thread::name;
        cout << ": " << _msg <<endl;
        //测试的是一个回显操作
        //问题：发送操作不能在计算线程中进行
        //
        //IO线程要与计算线程各司其职
        //_conn->send(_msg); // ==> 要将其打包成一个函数对象，交给EventLoop
        //对象进行执行
        //解决方案: 计算线程要通知IO线程进行发送
        
        // 1. wakeup() //给epoll的eventfd
        // 2. 向EventLoop对象注册回调函数(send) 
        _conn->sendInLoop(_msg);
    }
private:
    string _msg;
    const TcpConnectionPtr& _conn;
};

void onConnection(const TcpConnectionPtr& conn)
{
    cout << conn->toString() << " has connected!" <<endl;
}

void onMessage(const TcpConnectionPtr& conn)
{
    string msg = conn->receive();
    //业务逻辑的处理时间不宜过长
    //decode
    //compute
    //encode
    //
    // 如果时间过长，就需要使用线程池,主线程将计算Task放入线程池任务队列
    MyTask task(msg,conn); 
    // 使用的是基于对象的线程池
    gPool->addTask(std::bind(&MyTask::process, task));//给线程池执行计算

    // 计算线程要通知IO线程发送给客户端结果, 而不是自己直接进行发送
    /* conn->send(msg);//回文测试 */
}

void onClose(const TcpConnectionPtr& conn)
{
    cout << conn->toString() << " has closed" <<endl;
}

void test2()
{
    Acceptor acceptor("192.168.0.104",2000);
    acceptor.ready();

    EventLoop loop(acceptor);
    loop.setConnectionCallback(onConnection);
    loop.setMessageCallback(onMessage);
    loop.setCloseCallback(onClose);

    loop.loop();
}

void test3()
{
    ThreadPool pool(4,10);
    gPool = &pool;
    gPool->start();

    TcpServer server("192.168.0.104",2000);
    server.setConnectionCallback(onConnection);
    server.setMessageCallback(onMessage);
    server.setCloseCallback(onClose);
    server.start();
}

int main()
{
    test3();
    return 0;
}


#endif
