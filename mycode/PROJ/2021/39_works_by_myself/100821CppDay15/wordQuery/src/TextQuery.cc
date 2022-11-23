#include "../include/TextQuery.hh"
#include <sstream>

using std::cout;
using std::endl;
using std::stringstream;
using std::pair;
TextQuery::TextQuery(ifstream &ifs)
    : _spLines(new vector<string>){
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

QueryResult TextQuery::query(const string word) const{
    size_t cnt = _dict.count(word);
    auto it1 = _word2Numbers.find(word);
    if(0 == cnt){
        return QueryResult(word, _spLines, it1->second, shared_ptr<int>(new int(0)));
    }
    auto it = _dict.find(word);
    /* cout << "QueryResult query(const string word) const: " << it->first << *(it->second) << endl; */
    return QueryResult(word, _spLines, it1->second, it->second);
}

QueryResult TextQuery::query(const string word) {
    return QueryResult(word, _spLines, _word2Numbers[word], _dict[word]);
}


bool TextQuery::judgeWord(string word){
    for(auto &ch: word){
        if(!isalpha(ch)){
            return false;
        }
    }
    return true;
}
