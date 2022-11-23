#include<iostream>
#include<set>
#include<map>
using std::string;
using std::set;
using std::multimap;


//std::pair<int,int>countAndLine;
class Paragragh{
public:
    Paragragh(int line,string str)
    :_line(line)
    ,_data(str)
    {}
#if 1

    bool operator<(const Paragragh& p)const{
        return p._line>this->_line;
    }
#endif
    int _line;
    string _data;
};
class compPara{
public:
    bool operator()(const Paragragh& p1,const Paragragh& p2)const{
        return p1._line>p2._line;
    }
};
class TextQuery
{
public:
    void readFile(const string filename);
    void query(const string & word);
    void ReadFromFile(const string&filename);
    void getWordFromSet(const set<Paragragh>::iterator);
private:
    set<Paragragh>_strSet;
    multimap<string,std::pair<int,int>>strMap;
};
