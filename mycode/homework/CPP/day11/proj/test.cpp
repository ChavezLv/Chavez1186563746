#include<iostream>
#include<fstream>
#include<vector>
#include<map>
/*Stage1`target:
 * 1, 使用map,delim with whitespace,读取并存放单词以单词为键值-first;
 * 2, 过滤标点符号和数字,仅仅记录字符(65--->90&&97--->122);
 * 3,并且检查重复,记录重复次数在second位;
 * */
using namespace std;
int main(){
    cout<<"1"<<endl;
    ifstream fr;
    fr.open("china-daily.txt");
    //error: char* strBuffer[1024];
    string strBuffer;
    map<string,int>strMap;
    //map<int,string>strMap;
    int i=0;
    while(fr.good())
    {   //cout<<"2"<<endl;
        fr>>strBuffer;
        strMap.insert(pair<string,int>(strBuffer,i));
        //error:strMap.insert(i,strBuffer);

        //ok:strMap.insert(make_pair(i,strBuffer));
        //ok:strMap.insert(map<int,string>::value_type(i,strBuffer));
        //ok:strMap[i]=strBuffer;
        ++i;
    }
    for(map<string,int>::iterator it=strMap.begin();it!=strMap.end();++it){
        cout<<it->first<<":"<<it->second<<"\t";
        //cout<<"loop2"<<endl;
    }
    //cout<<"the account of words:"<<strMap.end()-strMap.begin()<<endl;
    cout<<"the account of words:"<<strMap.size()<<endl;
    cout<<"i="<<i<<endl;
    return 0;
}

