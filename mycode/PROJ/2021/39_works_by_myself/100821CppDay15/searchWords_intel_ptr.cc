#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>

#include <memory>

#include <fstream>
#include <sstream>

#include <set>
#include <map>

#include <algorithm>
/*
 *读取用户指定的任意文本文件【当前目录下的china_daily.txt】
 *查找单词: 查询的结果是该单词出现的次数, 并列出每次出现所在的行.行重复时
 *只显示一次, 行号升序
 * 
 *  vector<string> _lines; //存每一行
 * map<string, int> _dict; //保存单词以及词频
 * map<string, set<int> > _word2Numbers; //每个单词所在的行号, 查询时用到
 * */
using std::cin;
using std::cout;
using std::endl;
using std::string;

using std::ifstream;
using std::ofstream;
using std::stringstream;

using std::vector;
using std::pair;
using std::map;
using std::set;

using std::shared_ptr;

class QueryResult;

class TextQuery{
public:
    /* virtual */
    TextQuery(ifstream &ifs);
    bool judgeWord(string word);
    /* void read(const string &filename); */
    /* QueryResult query(const string word) const; */
    QueryResult query(string word) ;
    ~TextQuery(){

    }

private:
    /* vector<string> _lines; */
    shared_ptr< vector<string> > _spLines;
    map<string, shared_ptr<int>> _dict;
    map<string, shared_ptr<set<int>> > _word2Numbers; //每个单词所在的行号

};

class QueryResult{
    /* friend std::ostream & print(std::ostream& os, const QueryResult &rhs); */
    friend std::ostream & operator<<(std::ostream& os, const QueryResult &rhs);
public:
    QueryResult(string s, shared_ptr< vector<string> > spLines, \
                 shared_ptr< set<int> >  spWord2Numbers, shared_ptr<int> times)
    : _word(s)
    , _spLines(spLines)
    , _spLine(spWord2Numbers)
    , _times(times){ 

    }

private:
   string _word;
   shared_ptr< vector<string> > _spLines;
   shared_ptr< set<int> > _spLine;
   shared_ptr<int> _times;
   /* shared_ptr< map<string, set<int>> > _spWord2Numbers; */ 
};

TextQuery::TextQuery(ifstream &ifs)
: _spLines(new vector<string>){
    /* if(!ifs){ */
    /*     operator<<(cout, "file not exist"); */
    /*     return; */
    /* } */
    string line;
    string words;
    stringstream ss(line);
    while(getline(ifs, line)){
        ss.clear();
        ss << line;
        //0)存每一行
        _spLines->push_back(line);
        while(ss >> words){
            if(judgeWord(words)){
                //1)存词频
                auto ret = _dict.insert(pair<string, shared_ptr<int>>(words, new int(1)));
                if(!ret.second){
                    //如果插入不成功, 则++; 插入成功,不做处理
                    ++ *_dict[words];
                }
                //2)存行号
                /* set<int> tmp{(int)_lines.size()}; */
                shared_ptr<set<int>> tmp(new set<int>{(int)_spLines->size()});
                /* auto ret1 = _word2Numbers.insert(pair<string, set<int>>(words, tmp)); */
                auto ret1 = _word2Numbers.insert(pair<string, shared_ptr<set<int>>>(words, tmp));
                if(!ret1.second){
                    //如果插入不成功, 则已经存在, 需要根据键值插入
                    _word2Numbers[words]->insert((int)_spLines->size());
                }
            }
        }
    }
    
}

/* QueryResult TextQuery::query(const string word) const{ */
QueryResult TextQuery::query(string word) {
    /* cout << "element occurs " << _dict[word] << " times." << endl; */
    /* if(_dict[word]){ // 如果出现0次以上*/
    /*     /1* return QueryResult(word, _spLines, _word2Numbers[word]); *1/ */
    /*     /1* for(auto &elem :  _word2Numbers[word]){ *1/ */
    /*     /1*     cout << "(line " << elem << ")" << _lines[elem - 1] << endl; *1/ */ 
    /*     /1* } *1/ */
    /* }else{ */
    /*     /1* return QueryResult(word, _sp) *1/ */
    /* } */
    return QueryResult(word, _spLines, _word2Numbers[word], _dict[word]);
}

std::ostream & operator<<(std::ostream& os, const QueryResult &rhs){
    if(rhs._times){
        //出现多次,指针不为空
        os << "element occurs " <<  *rhs._times << " times." << endl;
    }else{
        //出现0次, 直接返回, 否则解引用段错误出错
        os << "element occurs " <<  "0" << " times." << endl;
        return os;
    }
    for(auto &elem : *(rhs._spLine)){
        os << "(line " << elem << ")" << (*rhs._spLines)[elem - 1] << endl; 
    }
    return os;
}

bool TextQuery::judgeWord(string word){
    for(auto &ch: word){
        if(!isalpha(ch)){
            return false;
        }
    }
    return true;
}

/* void TextQuery::read(const string &filename){ */
/*     ifstream ifs(filename, std::ios::in); */
/*     if(!ifs){ */
/*         operator<<(cout, "file not exist"); */
/*         return; */
/*     } */
/*     string line; */
/*     string words; */
/*     stringstream ss(line); */
/*     while(getline(ifs, line)){ */
/*         ss.clear(); */
/*         ss << line; */
/*         //0)存每一行 */
/*         _lines.push_back(line); */
/*         while(ss >> words){ */
/*             if(judgeWord(words)){ */
/*                 //1)存词频 */
/*                 auto ret = _dict.insert(pair<string, int>(words, 1)); */
/*                 if(!ret.second){ */
/*                     //如果插入不成功, 则++; 插入成功,不做处理 */
/*                     ++ _dict[words]; */
/*                 } */
/*                 //2)存行号 */
/*                 set<int> tmp{(int)_lines.size()}; */
/*                 auto ret1 = _word2Numbers.insert(pair<string, set<int>>(words, tmp)); */
/*                 if(!ret1.second){ */
/*                     //如果插入不成功, 则已经存在, 需要根据键值插入 */
/*                     _word2Numbers[words].insert((int)_lines.size()); */
/*                 } */
/*             } */
/*         } */
/*     } */
/*     ifs.close(); */
/* } */

/* void TextQuery::query(const string word){ */
/*     cout << "element occurs " << _dict[word] << " times." << endl; */
/*     for(auto &elem :  _word2Numbers[word]){ */
/*         cout << "(line " << elem << ")" << _lines[elem - 1] << endl; */ 
/*     } */
/* } */

/* void test(){ */
/*     string fileName; */
/*     cout << "please input finename you want to search:" << endl; */
/*     cin >> fileName; */
/*     TextQuery textQ; */
/*     textQ.read(fileName); */

/*     string word; */
/*     while(cout << "please input the word you want to search:" << endl, cin >> word){ */
/*         textQ.query(word); */
/*     } */

/* } */
void test(){
    string fileName;
    cout << "please input finename you want to search:" << endl;
    cin >> fileName;
    ifstream ifs(fileName, std::ios::in);
    if(!ifs){
        operator<<(cout, "file not exist");
        return;
    }
    TextQuery *textQ = new TextQuery(ifs);

    string word;
    while(cout << "please input the word you want to search:" << endl, cin >> word){
        cout << textQ->query(word) << endl;
        
    }
    delete textQ;
    ifs.close();
}
/* void test1(){ */
/*     string fileName; */
/*     cout << "please input finename you want to search:" << endl; */
/*     cin >> fileName; */
/*     ifstream ifs(fileName, std::ios::in); */
/*     if(!ifs){ */
/*         operator<<(cout, "file not exist"); */
/*         return; */
/*     } */
/*     TextQuery *textQ = new TextQuery(ifs); */

/*     string word; */
/*     cout << "please input the word you want to search:" << endl; */
/*     cin >> word; */
/*     cout << textQ->query(word) << endl; */
/*     delete textQ; */
/*     QueryResult qs(textQ->query(word)); */
/*     cout << qs << endl; */
        

/*     ifs.close(); */
/* } */
int main()
{
    test();
    return 0;
}

