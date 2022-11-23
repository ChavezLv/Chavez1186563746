#include<string.h>
#include<iostream>
using namespace std;
class CowString{
    class CharProxy{
    public:
        CharProxy(CowString &self,size_t idx)
            :_self(self),_idx(idx)
        {}
    char& operator=(char ch);
    operator char(){
        return _self._pstr[idx];
    }
    
    private:
        CowString& _self;
        size_t _idx;
    };
public:
    CowString()
        :_pstr(malloc()){
            initRefCount();
        }
    CowString(const char* pstr);
    CowString(const CowString &rhs);
    CowString&operator=(const CowString& rhs);
    ~CowString();
    CharProxy operator[](size_t idx)const{
        cout<<"operator[](size_t)const"<<endl;
        if(idx<size()){
    
        }
    }
};
