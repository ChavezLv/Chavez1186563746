#include<iostream>
#include<fstream>
#include<vector>
#include<map>
#include<set>
using namespace std;
/*Stage1`target:
 * 1, 使用map,delim with whitespace,读取并存放单词以单词为键值-first;
 * 2, 过滤标点符号和数字,仅仅记录字符(65--->90&&97--->122);
 * 3,并且检查重复,记录重复次数在second位;
 * 4, 程序将存储所打开文件的内容，以便输出每个单词所在的原始行。
 * */
class Paragragh{
public:
    Paragragh(int line,string str)
    :_line(line)
    ,_str(str)
    {}  
    //bool compPara(const Paragragh&p1,const Paragragh&p2);
    int _line;
    string _str;
    };
/*bool Paragragh::compPara(const Paragragh&p1,const Paragragh&p2){
    return p1._line-p2._line;
}*/
/*仿函数（Functor）又称为函数对象（Function Object）是一个能行使函数功能的类。
 *仿函数的语法几乎和我们普通的函数调用一样，不过作为仿函数的类，都必须重载 operator() 运算符*/
class compPara{
public:
    bool operator()(const Paragragh& p1,const Paragragh& p2){
        return p1._line-p2._line;//降序
    }
};
void testGetline(){
    ifstream fr;
    fr.open("china-daily.txt");
#if 0
    //利用set容器存储所在行
    //test1 result:not complete ok,按照首字符排序;解决方案,定义类;
    //error: char* strBuffer[1024];
    //debug:cout<<strBuffer<<endl;
    set<string>strSet;
    while(fr.good()){
        string strBuffer;
        getline(fr,strBuffer,'\n');
        strSet.insert(strBuffer);
    }
    for(set<string>::iterator it=strSet.begin();it!=strSet.end();++it){
        cout<<*it<<endl;
    }

#endif
#if 0
    //test2 result:ok
    int i=1;
    map<int,string>strMap;
    while(fr.good()){
        string strBuffer;
        getline(fr,strBuffer,'\n');
        strMap.insert(make_pair(i,strBuffer));
        ++i;
    }
    for(map<int,string>::iterator it=strMap.begin();it!=strMap.end();++it){
        cout<<it->first<<":"<<it->second<<endl;
    }
#endif
    //test3
  /*  typedef bool(*func)();
    func=&Paragragh::compPara;*/
    set<Paragragh,compPara>strSet;
    int i=1;
    while(fr.good()){
        string strBuffer;
        getline(fr,strBuffer,'\n');
        Paragragh tmp(i,strBuffer);
        strSet.insert(tmp);
        ++i;
    }
    Paragragh p(1,"dfa");
    strSet.find(p);
    for(set<Paragragh>::iterator it=strSet.begin();it!=strSet.end();++it){
        cout<<it->_line<<":"<<it->_str<<endl;
    }
}
void testLoad();
int main(){
    testGetline();
    //testLoad();
    return 0;
}
void testLoad(){
    //debug:cout<<"Begin"<<endl;
    ifstream fr;
    fr.open("china-daily.txt");
    //利用set容器存储所在行
    //error: char* strBuffer[1024];
    string strBuffer;
    map<string,int>strMap;
    //debug:map<int,string>strMap;
    int amount=0,Repeat=0;
    while(fr.good())
    {   //debug:cout<<"2"<<endl;
        fr>>strBuffer;
        int j=0;
        string strBuffer2;
        int Size=strBuffer.size();
        /*过滤单词*/
        for(j=0;j<Size;++j){
            //debug:cout<<strBuffer[j]<<endl;
            if((strBuffer[j]>='A'&&strBuffer[j]<='Z')\
              ||(strBuffer[j]>='a'&&strBuffer[j]<='z')||strBuffer[j]=='-'){
                strBuffer2+=strBuffer[j];
            }
            //debug:cout<<strBuffer2<<endl;
    }
        strMap.insert(pair<string,int>(strBuffer2,Repeat));
        //error:strMap.insert(i,strBuffer);

        //ok:strMap.insert(make_pair(i,strBuffer));
        //ok:strMap.insert(map<int,string>::value_type(i,strBuffer));
        //ok:strMap[i]=strBuffer;
        
        /*记录重复次数*/
        if(strMap.end()!=strMap.find(strBuffer2)){
            //find(),根据键值查找,成功返回当前元素迭代器,失败返回end迭代器
            ++strMap.find(strBuffer2)->second;
        }
        /*for(map<string,int>::iterator it=strMap.begin();it!=strMap.end();++it){
            if(it->first==strBuffer2){
               // ++Repeat;
                ++it->second;
            }
        }*/
        
        ++amount;
    }
    for(map<string,int>::iterator it=strMap.begin();it!=strMap.end();++it){
        cout<<it->first<<":"<<it->second<<"\t";
    }
    /*//验证单词数
    int tmp=0;
    int sum=0;
    for(map<string,int>::iterator it=strMap.begin();it!=strMap.end();++it){
        //debug:cout<<it->first<<":"<<it->second<<"\t";
        //重复次数大于1的单词
        if(it->second>1){
            cout<<it->first<<":"<<it->second<<endl;
//            ++tmp;
        }
        sum+=it->second;
        //debug:cout<<"loop2"<<endl;
    }*/


    //error:cout<<"the account of words:"<<strMap.end()-strMap.begin()<<endl;
    /*输出文件总单词数i,与不重复的单词数strMap.size()*/
    cout<<endl<<"the account of repeatitive words:"<<strMap.size()<<endl;
    cout<<"the amount of words="<<amount<<endl;
    //vertification:cout<<"sum="<<sum<<endl;
}

