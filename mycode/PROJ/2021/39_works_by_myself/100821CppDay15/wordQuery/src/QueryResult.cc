#include "../include/QueryResult.hh"

using std::endl;
QueryResult::QueryResult(string s, shared_ptr< vector<string> > spLines, \
                shared_ptr< set<int> >  spWord2Numbers, shared_ptr<int> times)
        : _word(s)
          , _spLines(spLines)
          , _spLine(spWord2Numbers)
          , _times(times){ }

std::ostream & operator<<(std::ostream& os, const QueryResult &rhs){
    if(0 < *rhs._times){
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
