 ///
 /// @file    TestTcpConnection.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-06-16 17:24:19
 ///
 
#include "Threadpool.hpp"
#include "TcpServer.hpp"
#include <unistd.h>
#include <signal.h>

#include <string>
#include <iostream>
#include <functional>
using std::cout;
using std::endl;
using namespace wd;

void cb1(const TcpConnectionPtr & conn) 
{
	//连接建立时要记录日志
	cout << conn->toString() << " has connected!" << endl;
} 
/*lcl: 后面涉及到缓存系统的设计,  笔记记载V4那里

onMessage把任务交个我们的线程池:  

计算线程process()
		//1. 执行业务逻辑的处理
		//decode
		//compute   如果有重复计算的时候, 是没有必要的
						Cache <key, value> 
					问题是: 如何拿到这个Cache?
					思考: 多个线程 ->  一对一的Cache
					假设: Cache使用vector进行管理, 
					vector< Cache <key, value> >
					那么需要, 每个线程都得有一个标志, 这个标识通过
					pthread_self()的返回值获得

					但是要和Cache产生关系, 就需要map< pthread_t, int> 
					pthread_t 的侵入性比较强, 因为要用到线程库的头文件
					这样就产生了耦合, 而且没办法解耦

					所以需要一个新的知识点, 线程局部存储TLS
					(Thread Local Storage)

					先看下Thread.hpp, 之前有一个一直存在的问题,
					都还没有很好地解决, pthread_t标记线程的可读性不是很好

					比如testThread.cc里面, 子线程可以用myTask类的process()和main()里面都可以
					调用pthread_self()来获取各自的id

					不过这样打印出来, 在打印界面纯数字也还表意不够清晰
					比如日志线程, 定时器线程, 计算线程, 这些更好, 可以通过宏
					所以线程可以考虑在
					Thread.hpp加上一个string _name变量

					现在线程有了_name数据成员, 和一个getName()函数
					但是现在在Task::process()除非获得线程对象的地址, 
					否则不可能在, Task::process()获得getName() 

					主线程可以通过getName()获得

					所以最终还是要用到, TLS  的 __thread变量 !!!
					
					看37分享代码(带注释版本)
					通过__thread POD's string = nullptr, 加 .cc里面extern
					就解决了pthread的名字的问题.

					线程名字, 回调函数, 都可以看成是线程特有的数据, 放到结构体里面去

					所以这个v4, 拿Cache, 对应一个整型数据获取下标就简单了

		//encode
*/
class Task
{
public:
	Task(const string & msg, const TcpConnectionPtr & conn)
	: _msg(msg)
	, _conn(conn)
	{
	
	}

	//process方法的执行在线程池中的某一个子线程进行: 
	//即某一个计算线程中运行
	void process()
	{
		cout << "Task::process() is running" << endl;
		//1. 执行业务逻辑的处理
		//decode
		//compute
		//encode

		//2. 将结果发送给客户端
		string response = _msg;//模拟回显服务
		//如果该操作能够正常执行，那就说明是在线程池中完成了
		//数据的发送，与模型不符合
		//
		//模型中，发送的数据的操作是在Reactor中完成的，
		//是在事件循环中完成的
		//
		//我们应该在这里，通知Reactor(IO线程)完成数据的发送
		//该怎么做？此时在计算线程中拥有的资源只有TcpConneciton对象
		//因此要通知IO线程，只能借助与TcpConnection对象
		//
		//当通知完成之后，Reactor要执行的任务是什么呢？
		//就是发送数据，即send方法的执行
		//那就需要将send方法包装成一个回调函数，注册给
		//Reactor对象所在的线程
		_conn->sendInLoop(response);

		//----------------补充rrr--------------------
		/* 补充: 
		_conn->send(response); 
		//按照职责的划分，计算线程并不负责数据的发送操作
		//在这里计算线程应该要通知IO线程进行数据的发送
		//使用eventfd来完成通知操作
				//唤醒IO线程, 就是EventLoop对象在loop方法中运行时调用send方法-->
		//1. 让epoll_wait返回  ==> 监听evenfd文件描述符
		//2. 之后再去执行_cond->send方法
		//
		//如何获取EventLoop对象?
		//只有TcpConnection知道被哪一个EventLoop进行管理
		// wakeup(); 

		////1. 拿到关键词之后，要先从缓存系统中查找
		//如何拿到缓存呢？ LRUCache ==> CacheManger中用vector存储
		// ==> 只需要获取到一个vector的下标就可以定位一个LRUCache
		
		//可行的方案
		//每一个线程的id是固定的   pthead_t --> LRUCache
		//map<pthread_t, int> _id2idxs;
		//pthread_self();
	
		
		//LRUCache & cache = CacheManager::getInstance()->getCache(pthread_self());
		//map<pthread_t, int>
		//LRUCache & cache = CacheManager::getInstance()->getCache(_id2idxs[pthread_self()]);
		
		int idx = atoi(wd::current_thread::name);
		cout << "idx : " << idx << endl;
		//LRUCache & cache = CacheManager::getInstance()->getCache(threadIdx);
		//---->[R/★]也就是说【几个cache】，实际上是【被cache类拥有的】，线程只有【依据自己的id拿到它们的资格】
		//存储线程对象为什么做不到这件事？因为【放一个index全局变量in线程】非常不优雅不美观啊！
		//
		//要想完美的解决该问题，就需要使用到一个新知识TLS（线程局部存储）--->线程的全局变量

		
		//2. 缓存中没有的时候，再去进行查询流程的执行

		_conn->sendInLoop(_msg);
		*/
		
	}

private:
	string _msg;
	TcpConnectionPtr _conn;
};

wd::Threadpool * gThreadpool = nullptr;

//网络编程过程中涉及到的三个事件
void onConnection(const TcpConnectionPtr & conn)
{
	cout << conn->toString() << " has connected!" << endl;
}

void onMessage(const TcpConnectionPtr & conn)//让其作为成员函数, 全局变量作为变量
{
	string msg = conn->receive();
	cout << "receive msg :" << msg << endl;
	//执行业务逻辑的处理的
	//但这个部分执行的时间不能过长， 10ms
	//1. 消息的处理最好是交给线程池来执行
	Task task(msg, conn);
	gThreadpool->addTask(std::bind(&Task::process, task));//需要一个全局变量	
	//2. 得到要返回给客户端的结果后，
	//再发送该返回结果
	//conn->send(msg);
}
 
void onClose(const TcpConnectionPtr & conn)
{
	cout << conn->toString() << " has closed!" << endl;
}

void test0() 
{
	using namespace wd;
	::signal(SIGPIPE, SIG_IGN);

	Threadpool threadpool(4, 10);
	threadpool.start();
	gThreadpool = &threadpool;//不是一个很好的方式, 代码里面出现了全局变量

	TcpServer server(8888);
	server.setAllCallbacks(onConnection, onMessage, onClose);
	server.start();
} 

//v4.1  后面做服务的时候, 就是线程池对象, tcp对象,
// 然后实现自己的三个回调函数, 在三个回调函数内部, 进行操作
class EchoServer
{
public:
	EchoServer(unsigned short port, const string & ip = "127.0.0.1")
	: _threadpool(4, 10) // 4, 10最好写在配置文件中
	, _server(port)
	{ }

	void start() {
		_threadpool.start();//先让线程池跑起来
		_server.setAllCallbacks(//注册回调函数, 用占位符, 因为此时参数不需要提供
			std::bind(&EchoServer::onConnection, this, std::placeholders::_1),
			std::bind(&EchoServer::onMessage, this, std::placeholders::_1),
			std::bind(&EchoServer::onClose, this, std::placeholders::_1));

		_server.start();//服务器启动
	}

	void onConnection(const TcpConnectionPtr & conn)
	{
		//toString有打印">> tcp "
		cout << ">> tcp " << conn->toString() << " has connected!" << endl;
	}

	void onMessage(const TcpConnectionPtr & conn)
	{//整个项目就是在这里, 
		string msg = conn->receive();

		//v4, 这一在这个位置, 还可以加入协议解析过程的代码, 非常灵活
		// 把msg解析出来, 具体要做什么, 也可以放到&Task::process里面
		Task task(msg, conn);

		//v4 为什么传对象, 不是传地址, 因为, 这个是栈对象, 不能传地址!
		_threadpool.addTask(std::bind(&Task::process, task));
	}

	void onClose(const TcpConnectionPtr & conn)
	{
		cout << ">> tcp " << conn->toString() << " has closed!" << endl;
	}

private:
	Threadpool _threadpool;
	TcpServer _server;
};

void test1()
{
	EchoServer server(8888);
	server.start();
}
 
int main(void)
{
	/* test0(); */
	test1();
	return 0;
}
