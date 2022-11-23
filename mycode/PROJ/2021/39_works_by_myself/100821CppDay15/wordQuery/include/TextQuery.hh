#ifndef __TEXTQUERY_HH__
#define __TEXTQUERY_HH__

#include "QueryResult.hh"
/* #include "Query.hh" */
#include <fstream>
#include <map>

using std::map;
using std::ifstream;

class NotQuery;
class AndQuery;
class OrQuery;
class QueryResult;
class TextQuery{
    friend NotQuery;
    friend AndQuery;
    friend OrQuery;
    /* friend QueryResult NotQuery::eval(const TextQuery &lhs) const; */
public:

    TextQuery(ifstream &ifs);
    bool judgeWord(string word);
    QueryResult query(const string word) const;
    QueryResult query(const string word) ;

private:
    shared_ptr< vector<string> > _spLines;//vector<string>指针
    map<string, shared_ptr<int>> _dict;// int指针
    map<string, shared_ptr<set<int>> > _word2Numbers; //每个单词所在的行号,set<int>指针

};

#endif
