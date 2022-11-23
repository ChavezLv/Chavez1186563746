#include <iostream>
using std::cin;
using std::cout;
using std::endl;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <map>
using std::map;

#include <algorithm>

#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>
#include <sys/epoll.h>

template<typename _Tp>
class Solution {
public:
   
};
void InetAddress(unsigned short port, const string & ip){
    struct sockaddr_in _addr;
    ::memset(&_addr, 0, sizeof(_addr));
    _addr.sin_family = AF_INET; 
    _addr.sin_port = htons(port);

    cout << _addr.sin_port << endl;
    cout << ((_addr.sin_port & (255 << 8)) >> 8) << endl;
    /* printf("%d\n", AF_INET); */
    /* printf("%x\n", _addr.sin_port); */


    _addr.sin_addr.s_addr = inet_addr(ip.c_str());
    cout << _addr.sin_addr.s_addr << endl;
    
    cout << (_addr.sin_addr.s_addr & 255) << ".";//127
    cout << ((_addr.sin_addr.s_addr & (255 << 8)) >> 8) << ".";//0看
    cout << ((_addr.sin_addr.s_addr & (255 << 8 << 8)) >> 16) << ".";//0看
    cout << ((_addr.sin_addr.s_addr & (255<< 24)) >> 24) << endl;//0看


}
void test01(){
    InetAddress(80, "127.0.0.1"); 
       
}
void test02(){
    vector<int> vec;
    vec.push_back(epoll_create1(0));
    vec.push_back(epoll_create1(0));
    vec.push_back(epoll_create1(0));
    vec.push_back(epoll_create1(0));
    for(auto &elem:vec){
        cout << elem << endl;
    }
}

void test03(){
    int s = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP);
    cout << IPPROTO_TCP << endl;
}
int main(int argc, char* argv[])
{
   
    test01();
    cout << "test02" << endl << endl;
    test02();
    cout << "test03" << endl << endl;
    test03();
    return 0;
}
/*================================================================
*   Copyright (C) 2021 ゞゞゞ All rights reserved.
*   
*   @file       ：testHtons.cc
*   @author     ：ゞゞゞ
*   @date       ：2021年08月28日 10:48:42
*   @description：
*
================================================================*/


