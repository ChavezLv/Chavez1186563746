#include "Client.hpp"
#include "../../include/InetAddress.hpp"

#include <iostream>
using std::cout;
using std::endl;
using std::cin;

namespace wd
{

Client::Client()
    :_sock()
     ,_socketIO(_sock.fd())
    {}

void Client::connect(const string& ip,unsigned short port)
{
    InetAddress serverAddr(ip,port);
    if(::connect(_sock.fd()
                 ,(struct sockaddr*)&*serverAddr.getInetAddressPtr()
                 ,sizeof(struct sockaddr)))
    {
        perror("connect");
    }
}

void Client::doServer()
{
    string word;
    char buf[1024] = {0};
    cout << "please input search word:" << endl;
    while(cin >> word)
    {
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

}//end of namespace spellCorrect

#if 1
using namespace spellCorrect;

int main()
{
    Client client;
    client.connect("192.168.0.104",2000);
    client.doServer();
    return 0;
}

#endif
