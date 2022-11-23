#include"query.hpp"
#include<fstream>
#include<cstring>
using namespace std;
void TextQuery::query(const string &word){
    //cout<<"noDefined"<<endl;
    multimap<string,pair<int,int>>::iterator itm1=strMap.find(word);
    pair<int,int>pair2;
    if(strMap.end()!=itm1){
        for(int i=0;i<100;++i)cout<<"-";
        pair2=itm1->second;
        cout<<endl<<"("<<itm1->first<<")"<<" occurs "<<pair2.first<<" times"<<endl;
        for(int i=0;i<100;++i)cout<<"-";
        cout<<endl;
        int amount=pair2.first;
        int line=-1;
        for(int i=0;i<amount;++i){
            set<Paragragh>::iterator its=_strSet.begin();
            pair<int,int>pair3 =itm1->second;
            //为了减少O(n)复杂度的for循环;应当使用map存储行号和段落的,仅为了学习使用set;
            for(;its!=_strSet.end();++its){
                if(pair3.second==its->_line&&line!=its->_line){
                    cout<<"line "<<pair3.second<<":"<<its->_data<<endl;
                    line=pair3.second;
                    break;
                }
            }
        for(int i=0;i<100;++i)cout<<"-";
        cout<<endl;

            ++itm1;
        }
    }
}

void TextQuery::readFile(const string filename){
    ifstream fr;
    fr.open(filename);
    //set<Paragragh,compPara>&strSet=_strSet;
    int i=1;
    /*将行号与段落从文件中读取,存储到set<Paragraph,compPara>中*/
    while(fr.good()){
        string strBuffer;
        getline(fr,strBuffer,'\n');
        Paragragh tmp(i,strBuffer);
        _strSet.insert(tmp);
        ++i;
    }
    /* test out:success!
     * for(set<Paragragh>::iterator it=_strSet.begin();\
          it!=_strSet.end();++it){
        cout<<it->_line<<":"<<it->_data<<endl;
    */

    /*从set中读取行号与段落,从段落内容中分离单词,储存每个单词的键值,出现次数,行号到multimap中*/
    set<Paragragh>::iterator it=_strSet.begin();
    int Repeat=1;
    for(;it!=_strSet.end();++it){
        //cout<<it->_line<<":"<<it->_data<<endl;   
        //利用str存储每一段的内容
        char* str=new char[(it->_data).length()+1];//size和lengh一样
        strcpy(str,it->_data.c_str());
        char* token=strtok(str," ");
        //从每一行中分离单词
        while(token){
            string strBuffer=token;
            token=strtok(NULL," ");
            string strBuffer2;
            int Size=strBuffer.size();
            //cout<<Size<<endl;
            //cout<<strBuffer<<endl;
            /*strBuffer获取的内容包含标点符号,滤除后存储到strBuffer2中*/
            for(int j=0;j<Size;++j){
                if((strBuffer[j]>='A'&&strBuffer[j]<='Z')\
                    ||(strBuffer[j]>='a'&&strBuffer[j]<='z')||strBuffer[j]=='-'){
                    strBuffer2+=strBuffer[j];
                }
	        }

            /*插入数据到multimap中*/
#if 1
            /*利用multimap.find();计算单词重复次数Repeat
             * map和set内部的find函数查找时间复杂度是O(logn)*/
            if(strMap.end()==strMap.find(strBuffer2)){
                /*第一次出现*/
                strMap.insert(pair<string,pair<int,int>>\
                    (strBuffer2,make_pair(Repeat,it->_line)));
            }else{
                /*第一行*/
                strMap.insert(pair<string,pair<int,int>>\
                    (strBuffer2,make_pair(Repeat,it->_line)));
                ++strMap.find(strBuffer2)->second.first;
            }
            //test:multimap<string,pair<int,int>>::iterator itm=strMap.begin();
            //test:cout<<itm->first<<":"<<itm->second.first<<":"<<itm->second.second<<endl;
            
        }
#endif
        delete []str;
#if 0
    //遍历multimap容器获取单词存储信息;
    //multimap<string,pair<int,int>>::iterator itr=strMap.begin();
    for(auto itr=strMap.begin();itr!=strMap.end();++itr){
        cout<<itr->first<<":amount="<<itr->second.first\
            <<",line="<<itr->second.second<<"\t";
    }
    cout<<endl;
#endif
    }
}
    
int main(){
    string filename="china-daily.txt";
    TextQuery tq;
    tq.readFile(filename);
    tq.query("China");
    return 0;
}
