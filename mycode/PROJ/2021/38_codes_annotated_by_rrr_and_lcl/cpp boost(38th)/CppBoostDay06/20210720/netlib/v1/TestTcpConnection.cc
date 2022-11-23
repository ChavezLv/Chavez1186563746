 ///
 /// @file    TestTcpConnection.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-06-16 17:24:19
 ///
 
#include "Acceptor.hpp"
#include "TcpConnection.hpp"

#include <unistd.h>
#include <signal.h>

#include <iostream>
using std::cout;
using std::endl;
 
void test0() 
{
	/* ::signal(SIGPIPE, SIG_IGN); */

	using namespace wd;
	Acceptor acceptor(8888);
	acceptor.ready();

	TcpConnection conn(acceptor.accept());
	cout << conn.toString() << " has connected!" << endl;

	cout << "server receive: " << conn.receive() << endl;
	conn.send("welcome to server.!");

#if 0
	::sleep(2);
	cout << "first send: " << endl;
	conn.send("welcome to server.!");
	::sleep(2);

	cout << "second send: " << endl;
	conn.send("welcome to server.!");

	cout << "1111 " << endl;
#endif
} 
 
int main(void)
{
	test0();
	return 0;
}
