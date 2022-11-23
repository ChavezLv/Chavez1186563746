#pragma once
#include <cstdio>
#include <typeinfo>
#include <ctype.h>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;
using std::vector;
using std::pair;
using std::string;
using std::map;
using std::set;
using std::unordered_map;
using std::ios;
using std::ifstream;
using std::istringstream;
class Dictionary
{
public:
    Dictionary(){};
    ~Dictionary(){};
    //init函数将字典和索引分别读到
    //vector<pair<string,int> > _dict;
    //unordered_map<string,set<int> > _index;
    void init(const string &dictpath1,const string &dictpath2)
    {
        //只写打开字典文件,索引文件
        ifstream myfileD(dictpath1.c_str(),ios::in);
        ifstream myfileI(dictpath2.c_str(),ios::in);
        //str存储读取到的一行字符串
        string str;
        //word存储一行中的单个词语
        string word;
        //tempPair存储单个vector数据包括一个词和该次的词频
        pair<string,int>tempPair;
        //截取词和词频,
        while(getline(myfileD,str)) {
            istringstream line(str);
            line>>word;
            tempPair.first=word;
            line>>word;
            tempPair.second=atoi(word.c_str());
            _dict.push_back(tempPair);
        }   
        //first存储索引文件的每一行的第一个关键字,即_index的first变量
        string first;
        //tempSet存储_dict含有first关键字的所有行
        set<int>tempSet;
        
        while(getline(myfileI,str))
        {   
            istringstream line(str);
            line>>word;
            first=word;
            while(line>>word)
            {   
                tempSet.insert(atoi(word.c_str()));
            }   
            _index[first]=tempSet;
            //清空tempSet,以便复用
            tempSet.clear();
        }   
    }
    //外部获取字典
    vector<pair<string,int>> &getDict()
    {
        return _dict;
    }
    //外部获取索引
    unordered_map<string,set<int> >&getIndexTable()
    {
        return _index;
    }
    //执行查询,返回的是所有未可选未计算最小编辑距离的词汇
    vector<string> doQuery(string word)
    {
        vector<string> temp;
        set<int> res;
        string s;
        int i=1;
        int flag=0;
        while(flag<(int)word.size())
        {
            s=sub(word,i,flag,i);
            if(_index.count(s))
            {
                for(auto num:_index[s])
                {
                    res.insert(num);
                }
            }
            i++;
        }
        for(auto n:res)
        {
            temp.push_back(_dict[n].first);
        }
        return temp;
    }
    //为sub函数所用判断字符是否是汉字
    int is_zh_ch(char p)
    {

        /*汉字的两个字节的最高为都为1,这里采用判断最高位的方法
          将p字节进行移位运算，右移8位，这样，如果移位后是0，
          则说明原来的字节最高位为0，不是1那么也就不是汉字的一个字节
          */
        if (~(p >> 8) == 0)
        {
            return 1;//代表不是汉字
        }

        return -1;
    }
    //可以将中文,英文,中英文字符串逐单个提取出来
    string sub(string str, int start,int &flag ,int end = -1)
    {
        //flag为外部传入的标志位,来判断是否可以结束外部的循环
        //因为汉字和字母字符长度不一,不能简单按长度遍历
        if (typeid(str) == typeid(string) && str.length() > 0)
        {
            int len = str.length();

            string tmp = "";

            //先把str里的汉字和英文分开
            vector <string> dump;
            int i = 0;
            while (i < len)
            {
                if (is_zh_ch(str.at(i)) == 1)
                {
                    dump.push_back(str.substr(i, 3));
                    i = i + 3;

                }
                else
                {
                    dump.push_back(str.substr(i, 1));
                    i = i + 1;
                }
            }
            end = end > 0 ? end : dump.size(); //end默认为dump.size
            if (start<0 || start>end)
                printf("start is wrong");
            //直接从dump里取即可
            for (i = start; i <= end; i++)
            {
                tmp += dump[i - 1];
            }
            //size()>1表示此次查询是汉字,flag+=3
            if(tmp.size()>1)flag+=3;
            //否则为英语字符+=1
            else flag+=1;
            return tmp;
        }
        else
        {
            printf("str is not string\n");
            return "";
        }
    }
private:
    vector<pair<string,int> > _dict;
    unordered_map<string,set<int> > _index;
    static Dictionary *_pInstance;
};

