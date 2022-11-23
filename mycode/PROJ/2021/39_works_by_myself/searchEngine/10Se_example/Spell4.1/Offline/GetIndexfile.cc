#include "GetIndexfile.hh"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

//取出词典
void GetIndexFile::getaimsdir()
{
    ifstream ifs(_aimsdir);
    if(!ifs)
    {
        cout << "词典文件出错" << endl;
    }
    string line;
    while(getline(ifs,line))
    {
        istringstream iss(line);
        string key;
        int value;
        iss>>key>>value;
        _dict.push_back(make_pair(key,value));
    }
    ifs.close();
}

//取出停用词
void GetIndexFile::getstopdir()
{
    ifstream ifs(_stopdir);
    if(!ifs)
    {
        cout << "停用词文件出错" << endl;
    }
    string line;
    while(ifs>>line)
    {
        _stopWords.insert(line);
    }
    ifs.close();
}

//创建索引文件
void GetIndexFile::create_index()
{
    cout << "创建索引文件" << endl;
    for(size_t i=0;i!=_dict.size();++i)
    {
        string tmp=_dict[i].first;
        if(isEnglish(tmp)) //英文
        {
            if(_stopWords.find(tmp) == _stopWords.end())
            {
                for(auto c:tmp)
                {
                    string charactor(1,c);
                    if(isalpha(c))
                    {
                        auto cit = _index.find(charactor);
                        if(cit == _index.end())
                        {
                            set<int> smp;
                            smp.insert(i+1);
                            _index.insert(make_pair(charactor,smp));
                        }
                        else //存在该字母索引
                        {
                            cit->second.insert(i+1);
                        }
                    }
                }
            }
        }
        else //中文
        {   
            vector<string>word;
            auto cit =tmp.begin();
            while(cit<tmp.end())
            {
                string oneCharacter;
                oneCharacter.append(cit,cit+3);
                word.push_back(oneCharacter);
                cit +=3;
            }

            for(auto oneword:word)
            {
                if(_stopWords.find(oneword) == _stopWords.end())
                {
                    auto cit =_index.find(oneword);
                    if(cit == _index.end())
                    {
                        set<int> smp;
                        smp.insert(i+1);
                        _index.insert(make_pair(oneword,smp));
                    }
                    else
                    {
                        cit->second.insert(i+1);
                    }
                }
            }
        }
    }
}

//将索引文件写入文件
void GetIndexFile::store_index()
{
    cout << "我要写入索引文件了" << endl;
    ofstream ofs(_indexdir);
    if(!ofs)
    {
        cout << "索引文件打开失败" << endl;
    }
    else
    {
        for(auto data:_index)
        {
            ofs << data.first << " ";
            for(auto linenum:data.second)
            {
                ofs << linenum <<" ";
            }
            ofs << endl;
        }
        ofs.close();
    }
}

//判断是英文
bool GetIndexFile::isEnglish(const string &rhs) const
{
    char c=*(rhs.begin());
    if(c<0)
    {
        return false;
    }
    return true;
}
