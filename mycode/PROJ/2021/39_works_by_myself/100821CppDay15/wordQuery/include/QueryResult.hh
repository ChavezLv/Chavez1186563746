#ifndef __QUERYRESULT_HH__
#define __QUERYRESULT_HH__

#include <memory>
#include <string>
#include <vector>
#include <set>
#include <iostream>

using std::string;
using std::vector;
using std::set;
using std::shared_ptr;

class QueryResult{
    /* friend std::ostream & print(std::ostream& os, const QueryResult &rhs); */
    friend std::ostream & operator<<(std::ostream& os, const QueryResult &rhs);//重载<<
public:
    QueryResult(string s, shared_ptr< vector<string> > spLines, \
                shared_ptr< set<int> >  spWord2Numbers, shared_ptr<int> times);

    /* set<int>::size_type size() const{ */
    /*     return _spLine->size(); */
    /* } */
    vector<string>::size_type size() const{
        return _spLines->size();
    }

    shared_ptr<vector<string>> getFile(){
        return _spLines;
    }

    set<int>::const_iterator begin() const{
        return _spLine->begin();
    }

    set<int>::const_iterator end() const{
        return _spLine->end();
    }
private:
    string _word;
    shared_ptr< vector<string> > _spLines;
    shared_ptr< set<int> > _spLine;
    shared_ptr<int> _times;

};

#endif
