#ifndef QUERY_H
#define QUERY_H
#include "TextQuery.hh"
#include <string>
#include <set>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>

#include <algorithm>
using std::set_intersection;

using std::cout;
using std::endl;

//查询类的abstract class
class QueryBase{
    friend class Query; 
protected:
    virtual
    ~QueryBase() {  }
private:
    virtual
    QueryResult eval(const TextQuery&) const = 0;

    virtual
    string rep() const = 0;
};

//接口类
class Query{
    friend Query operator~(const Query &);
    friend Query operator|(const Query &, const Query&);
    friend Query operator&(const Query &, const Query&);

public:
    Query(const string&);
    
    QueryResult eval(const TextQuery& lhs) const{
        return _q->eval(lhs);
    }

    string rep() const{
        return _q->rep();
    }

private:
    Query(shared_ptr<QueryBase> query)
    : _q(query){

    }

private:
    shared_ptr<QueryBase> _q;
};

inline
std::ostream & operator<<(std::ostream &os, const Query &query){
    //cout << query.rep(); // 不需要友元类
    os << query.rep();
    return os; 
}

class WordQuery
: public QueryBase{
    friend class Query;
private:
    WordQuery(const string &word)
    : _word(word){
        /* cout << "WordQuery(const string &word)" << endl; */
    }
    
    QueryResult eval(const TextQuery &lhs) const{
        /* cout << "QueryResult eval(const TextQuery &lhs) const" << endl; */
        
        //没有重载const query时
        /* TextQuery* plhs = const_cast<TextQuery*>(&lhs); */
        /* return plhs->query(_word); */

        return lhs.query(_word);//有重载const query时 
    }

    string rep() const{
        return _word;
    }
    
private:
    string _word;
};

inline
Query::Query(const string &rhs)
: _q(new WordQuery(rhs)){
    /* cout << "Query(const string &rhs)" << endl; */

}

class NotQuery
: public QueryBase{
    friend Query operator~(const Query&);
private:
    NotQuery(const Query &q)
    :  _q(q) {

    }

    QueryResult eval(const TextQuery &) const;

    string rep()const {
        return "~( " + _q.rep() +" )"; 
    }
private:
    Query _q;
};

QueryResult NotQuery::eval(const TextQuery &lhs)const{
    QueryResult queryRes = _q.eval(lhs);
    shared_ptr<set<int>> spSet(new set<int>); 

    set<int>::const_iterator beg = queryRes.begin();
    set<int>::const_iterator end = queryRes.end();
    vector<string>::size_type vecSize = queryRes.size();

    int cnt = 0;

    /* for(auto it = beg; it != end; ++it){ */
    /*     cout << *it << " "; */
    /* } */
    /* cout << endl; */
    
    for(size_t i = 0; i != vecSize; ++i){
        /* if(beg == end || *beg != (int)i){// 当存在行与当前遍历不相等时插入 */
        if((*beg - 1) != (int)i){// 当存在行的下标(行号-1)与当前遍历下标不相等时插入
            spSet->insert(i + 1);//i 时 行集合的下标, i + 1 是行号
            ++ cnt;
        }else if(beg != end){
            ++beg;
        }
    }

    /* for(auto &el : *spSet){ */
    /*     cout << el << " " ; */
    /* } */
    /* cout << endl; */

    /* string word = rep(); */
    /* auto it = lhs._dict.find(word);//map::iterator */
    /* return QueryResult(rep(), queryRes.getFile(), spSet, it->second); */
    return QueryResult(rep(), queryRes.getFile(), spSet, shared_ptr<int>(new int(cnt)));//也可以用行总数减去出现的次数
}

class BinaryQuery
: public QueryBase{
protected:
    BinaryQuery(const Query &lhs, const Query &rhs, string operatorStr)
    : _lhs(lhs)
    , _rhs(rhs)
    , _operatorStr(operatorStr){

    }

    string rep() const {
        return "(" + _lhs.rep() + " " +\
            _operatorStr + " " + _rhs.rep() + ")";
    }
protected:
    Query _lhs, _rhs;
    string _operatorStr;
};

class AndQuery
: public BinaryQuery{
    friend Query operator&(const Query&, const Query&);
private:
    AndQuery(const Query &lhs, const Query &rhs)
    : BinaryQuery(lhs, rhs, "&"){

    }

    QueryResult eval(const TextQuery&) const;
};

QueryResult AndQuery::eval(const TextQuery& lhs) const{
    QueryResult _lhsRes = _lhs.eval(lhs);
    QueryResult _rhsRes = _rhs.eval(lhs);

    shared_ptr<set<int>> spSet(new set<int>); 
    
    set_intersection(_lhsRes.begin(), _lhsRes.end(), _rhsRes.begin(), _rhsRes.end(),\
                     inserter(*spSet, spSet->begin()));

    /* auto it = lhs._dict.find(rep());//map::iterator , 旧的行集合*/
    size_t sz = spSet->size();//新的行集合数量
    return QueryResult(rep(), _lhsRes.getFile(), spSet, shared_ptr<int>(new int(sz)));
}

class OrQuery
: public BinaryQuery{
    friend Query operator|(const Query&, const Query&);
private:
    OrQuery(const Query &lhs, const Query &rhs)
    : BinaryQuery(lhs, rhs, "|"){

    }
    
    QueryResult eval(const TextQuery&) const;
};
QueryResult OrQuery::eval(const TextQuery& rhs) const{
    QueryResult _lhsRes = _lhs.eval(rhs); 
    QueryResult _rhsRes = _rhs.eval(rhs); 
    

    shared_ptr< set<int> > spSet(new set<int>(_lhsRes.begin(), _lhsRes.end()));
    
    spSet->insert(_rhsRes.begin(), _rhsRes.end());

    /* auto it = rhs._dict.find(rep());//map::iterator */
    size_t sz = spSet->size();//新的行集合数量

    /* return QueryResult(rep(), _lhsRes.getFile(), spSet, it->second); */
    return QueryResult(rep(), _lhsRes.getFile(), spSet, shared_ptr<int>(new int(sz)));
}

//这里发生了隐式转换
inline
Query operator&(const Query &lhs, const Query &rhs){
    return shared_ptr<QueryBase>(new AndQuery(lhs, rhs));
}

inline
Query operator|(const Query &lhs, const Query &rhs){
    return shared_ptr<QueryBase>(new OrQuery(lhs, rhs));
}

inline
Query operator~(const Query &lhs){
    return shared_ptr<QueryBase>(new NotQuery(lhs));
}

ifstream & openFile(ifstream&, const string &);
TextQuery getFile(int, char**);
bool getWord(string&);
bool getWords(string &, string&);
std::ostream & operator<<(std::ostream& os, const QueryResult &rhs);

TextQuery getFile(int argc, char** filename){
    /* std::cout << argc << std::endl; */
    /* std::cout << filename[1] << filename[2] << std::endl; */
    ifstream ifs(filename[argc - 1], std::ios::in);
    if(!ifs){
        operator<<(std::cout ," file not exist");
        
    }
    TextQuery textQ(ifs);
    return textQ;
}

bool getWord(string& word){
    std::cin >> word;
    if(std::cin.eof()){
        return false;
    }
    return true;
}
bool getWords(string &word1, string& word2){
    std::cin >> word1 >> word2;
    if(std::cin.eof()){
        return false;
    }
    return true;
}
#endif
