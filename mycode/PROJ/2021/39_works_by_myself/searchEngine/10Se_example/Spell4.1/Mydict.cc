#include "Mydict.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <unistd.h>

using std::ifstream;
using std::istringstream;
using std::cout;
using std::endl;
namespace wd{
Mydict* Mydict::_mydict=nullptr;
pthread_once_t Mydict::_once=PTHREAD_ONCE_INIT;

void Mydict::initIndexTable(const string& indexPath,const string&dirpath){
   ifstream ifs(indexPath);
   ifstream ifs1(dirpath);
   
   
   cout<<"indexTable path is "<<indexPath<<endl;
   cout<<"dirpath path is "<<dirpath<<endl;
   if(ifs.bad()){
        perror("ifstream");
   }
   
   string line;
   /* int i=0; */
   //读取字典
   while(getline(ifs1,line)){
       istringstream iss(line);
       string word,freq;
       
       iss>>word>>freq;
       _dict.push_back(make_pair(word,stoi(freq)));
   }
   //读取索引表
   while(getline(ifs,line)){
       /* cout<<"init  index "<<endl; */
       
       istringstream iss(line);
       string word,freq;
        
       iss>>word;
       /* _dict.push_back(make_pair(word,stoi(freq))); */ 
       while(iss>>freq){
            /* cout<<"word ="<<word<<endl; */
            /* cout<<"freq ="<<freq<<endl; */
            _indexTable[word].insert(stoi(freq)); 
    }
       /* int idx=0; */
       /* for(int i=0;i<length(word);++i){ */
       /*     int curSize=nByte(word[idx]); */
       /*     string s=word.substr(idx,curSize); */
       /*     idx+=curSize; */
       /*     _indexTable[s].insert(i); */ 
       /* } */
       /* ++i; */
   }
   
   cout<<"··············indexTable·················"<<endl;
   /* cout<<"i = "<<i<<endl; */
   cout<<"_dict size = "<<_dict.size()<<endl;
   cout<<"_indexTable size ="<<_indexTable.size()<<endl;
   cout<<"美 has occurs "<<_indexTable["美"].size()<<endl;
   cout<<"··············indexTable·················"<<endl;

    ifs.close();
    ifs1.close();
}
int Mydict::nByte(const char&c){
    int ans=1;
    if((c&(1<<7))){
        for(int i=0;i<=5;++i){
            int idx=6-i;
            if((c&(1<<idx))){
               ++ans; 
            }
            else{
                break;
            }
        }
    }
    
    return ans;
}
int Mydict::length(const string&s){
    int n=0;
    for(int i=0;i<s.size();){
        i+=nByte(s[i]);
        ++n;
    }
    return n;
}
}//end of namespace;
