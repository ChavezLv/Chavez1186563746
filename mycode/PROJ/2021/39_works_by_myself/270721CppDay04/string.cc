#include <string.h>
#include <iostream>
#include <vector>
#include <algorithm>
/* using namespace std; */
using std::cout;
using std::endl;

class String {
public:
    String();
    String(const char *rhs);
    String(const String &rhs);
    String &operator=(const String &rhs);
    ~String();
    void print();
private:
    char *_pstr;
};

String::String()
: _pstr(nullptr){
    cout << "String()" << endl;
}

String::String(const char *rhs)
: _pstr(new char[strlen(rhs) + 1]()){
    strcpy(_pstr, rhs);
    cout << "String(const char *rhs)" << endl;
}

String::String(const String &rhs)
:_pstr(new char[strlen(rhs._pstr) + 1]()){
    strcpy(_pstr, rhs._pstr);
}

String& String::operator=(const String &rhs){
    //1)自复制
    if(this == &rhs){
        return *this;
    }
    cout << "operator=(const String *rhs)" << sizeof(*_pstr) << endl;
    if(strlen(_pstr) >= strlen(rhs._pstr)){
        /* memset(_pstr, 0, strlen(_pstr)); */
        //strlen(_pstr) error
        memset(_pstr, 0, sizeof(*_pstr));
    }else{
        delete []_pstr;
        _pstr = nullptr;
        if(!_pstr){
            _pstr = new char[strlen(rhs._pstr) + 1]();
        }
    }
    memmove(_pstr, rhs._pstr, strlen(rhs._pstr) + 1);

    return *this;
}

void String::print(){
    if(_pstr){
        operator<<(cout, "print():not nullptr")<< _pstr << endl;
    }else{
        cout << "print():nullptr" << endl;
    }
}

String::~String(){
    cout << "~String()" << _pstr << endl;
    if(nullptr != _pstr){
        delete []_pstr;
        _pstr = nullptr;
    }
}
int main()
{
    String str1;
    str1.print();

    String str2 = "Hello, world";
    String str3("wangdao");

    str2.print();
    str3.print();

    String str4 = str3;
    str4.print();

    str4 = str2;
    str4.print();
    return 0;
}

