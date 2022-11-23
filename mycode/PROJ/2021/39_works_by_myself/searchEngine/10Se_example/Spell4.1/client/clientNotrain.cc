#include <iostream>
#include <func.h>

#pragma comment(lib, "lib_json.lib")
#include "json/json.h"

using namespace::std;
void displayJSONdata(char*);
int main()
{
    int peerfd=socket(PF_INET,SOCK_STREAM,0);
    if(peerfd==-1){
        perror("socket");
    }

    struct sockaddr_in addr;
    memset(&addr,0,sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_port=htons(8888);
    addr.sin_addr.s_addr=inet_addr("172.17.147.39");
    /* addr.sin_addr.s_addr=INADDR_ANY; */
    

    int ret=connect(peerfd,(struct sockaddr*)&addr,sizeof(addr));
    if(ret==-1){
        perror("connect");
    }

    char buf[1024]={0};
    recv(peerfd,buf,sizeof(buf),0);
    cout<<buf<<endl;
    while(1){
        memset(buf,0,sizeof(buf));
        
        read(STDIN_FILENO,buf,sizeof(buf));
        cout<<"查询词 :"<<buf<<endl;
        send(peerfd,buf,strlen(buf),0);

        memset(buf,0,sizeof(buf));
        recv(peerfd,buf,sizeof(buf),0);
        
        displayJSONdata(buf);
        cout<<endl;
        /* cout<<"recv from server "<<buf<<endl; */
    }
    close(peerfd);
    return 0;
}
void displayJSONdata(char* buf){
    string strValue(buf);
    /* cout<<strValue<<endl; */
    Json::Reader reader;
    Json::Value value;

    if (reader.parse(strValue, value))
    {
        cout<<"query word is "<<value["word"].asString()<<endl;
        int n=value["array"].size();
        /* cout<<"size = "<<n<<endl; */
        cout<<"maybe word is ";
        for(int i=0;i<n;++i){
            cout<<value["array"][i].asString()<<" ";
        }
        cout<<endl;
        
    }
}

