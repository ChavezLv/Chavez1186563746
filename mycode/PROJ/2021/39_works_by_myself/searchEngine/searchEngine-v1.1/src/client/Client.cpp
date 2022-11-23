/*================================================================
*   Copyright (C) 2021 anonymous All rights reserved.
*   
*   @file       ：Client.cpp
*   @author     ：anonymous
*   @date       ：2021年09月02日 20:02:35
*   @description：
*
================================================================*/


#include "Client.hpp"
#include "InetAddress.hpp"
#include "EventLoop.hpp"
#include <iostream>
using std::cout;
using std::endl;
using std::cin;

namespace wd{

Client::Client()
    :_sock()
     ,_socketIO(_sock.fd())
    {}

void Client::connect(const string& ip,unsigned short port)
{
    InetAddress serverAddr(ip,port);
    if(0 != ::connect(_sock.fd(), (struct \
                              sockaddr*)&*serverAddr.getInetAddressPtr(), sizeof(struct sockaddr)))
    {
        perror("connect");
        exit(0) ;//未连接则退出
    }
}

void Client::doServer()
{
    string word;
    char buf[1024] = {0};
    while(cin >> word)
    {
        cout << "please input search word:" << endl;
        _socketIO.writeTrain(word.c_str(),word.size());
        int ret = _socketIO.readTrain(buf);
        if(0 == ret)//连接断开
        {
            cout << "server close!" << endl;
            break;
        }
        else
        {
            string result(buf);
            cout << "from server: " << result << endl;;
        }
    }
}

}//end of namespace wd

#if 1
using namespace wd;

int main()
{
    Client client;
    client.connect("192.168.10.132", 8888);
    /* client.doServer(); */
    EventLoop eventLoop(client.fd());
    eventLoop.loop();

    eventLoop.unloop();
    return 0;
}

#endif



