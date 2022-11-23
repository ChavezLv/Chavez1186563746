#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>

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

class TextQuery{
public:
    /* virtual */
    bool judgeWord(string word);
    void read(const string &filename);
    void query(string word);

private:
    vector<string> _lines;
    map<string, int> _dict;
    map<string, set<int> > _word2Numbers; //每个单词所在的行号

};

bool TextQuery::judgeWord(string word){
    for(auto &ch: word){
        if(!isalpha(ch)){
            return false;
        }
    }
    return true;
}

void TextQuery::read(const string &filename){
    ifstream ifs(filename, std::ios::in);
    if(!ifs){
        operator<<(cout, "file not exist");
        return;
    }
    string line;
    string words;
    stringstream ss(line);
    while(getline(ifs, line)){
        ss.clear();
        ss << line;
        //0)存每一行
        _lines.push_back(line);
        while(ss >> words){
            if(judgeWord(words)){
                //1)存词频
                auto ret = _dict.insert(pair<string, int>(words, 1));
                if(!ret.second){
                    //如果插入不成功, 则++; 插入成功,不做处理
                    ++ _dict[words];
                }
                //2)存行号
                set<int> tmp{(int)_lines.size()};
                auto ret1 = _word2Numbers.insert(pair<string, set<int>>(words, tmp));
                if(!ret1.second){
                    //如果插入不成功, 则已经存在, 需要根据键值插入
                    _word2Numbers[words].insert((int)_lines.size());
                }
            }
        }
    }
    ifs.close();
}

void TextQuery::query(string word){
    cout << "element occurs " << _dict[word] << " times." << endl;
    for(auto &elem :  _word2Numbers[word]){
        cout << "(line " << elem << ")" << _lines[elem - 1] << endl; 
    }
}

void test(){
    string fileName;
    cout << "please input finename you want to search:" << endl;
    cin >> fileName;
    TextQuery textQ;
    textQ.read(fileName);

    string word;
    while(cout << "please input the word you want to search:" << endl, cin >> word){
        textQ.query(word);
    }

}
int main()
{
    test();
    return 0;
}

