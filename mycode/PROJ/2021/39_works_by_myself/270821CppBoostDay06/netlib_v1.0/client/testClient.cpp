/*================================================================
*   
*   @file       ：testClient.cpp
*   @date       ：2021年08月27日 23:06:16
*   @description：
*
================================================================*/
#include "Connect.h"
/* #include "../SocketIO.h" */

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

template<typename _Tp>
class Solution {
public:
   
};
void test01(){
    wd::Connect conn(6666);
    int sfd = conn.ready();
    if(sfd < 0 ){
        cout << sfd << endl;
        return;
    }
    cout << "Connecting to 192.168.10.132:6666..." << endl;
    wd::SocketIO io(sfd);
    string line;
        char buf[2<<6] = { 0 };
        /* io.readline(buf, sizeof(buf)); */
        /* cout << buf << endl; */
    while(1){
        cout << ">>pls input : " << endl;
        getline(cin, line);
        line.append("\n");
        io.writen(line.c_str(), line.size());  
        
        /* char buf[2<<6] = { 0 }; */
        io.readline(buf, sizeof(buf));
        cout << buf << endl;



        

    }

       
}
int main(int argc, char* argv[])
{
   
       test01();
          return 0;
}


