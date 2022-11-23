/*================================================================
*   
*   @file       ：testTcpConnetion.cpp
*   @date       ：2021年08月27日 22:42:16
*   @description：
*
================================================================*/
#include "TcpConnection.h"
#include "Acceptor.h"

#include <signal.h>
#include <iostream>
using std::cout;
using std::endl;

using namespace wd;
void test01(){
    Acceptor ac(6666);
    ac.ready();

    int newfd = ac.accept();
    cout << "main  accept:" << newfd << endl;


    /* TcpConnection conn(ac.accept()); */
    TcpConnection conn(newfd);
    cout << conn.toString() << " has connected." << endl;

        conn.send("welcome to Ubuntu 18.04.5 LTS \
              (GNU/Linux 4.15.0-154-generic x86_64)");
    while(1){
        //接
        cout << "server receive: " << conn.receive() << endl;

        //发
        conn.send("welcome to Ubuntu 18.04.5 LTS \
              (GNU/Linux 4.15.0-154-generic x86_64)");

    }
}
int main(int argc, char* argv[])
{

    test01();
    return 0;
}


