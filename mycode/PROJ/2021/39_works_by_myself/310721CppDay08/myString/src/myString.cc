#include "../include/myString.hh"

/* String::String() */
/* : _pstr(new char[1]()){ */
/*     operator<<(cout, "String()")<< endl; */
/*     _pstr[0] = '\0'; */
/* } */

String::String(const char *lhs)
/* : _pstr(new char[strlen(lhs) + 1]())// 如果传nullptr core dump */
{
    cout << "String(const char *lhs)" << endl;    
    if(!lhs){
        _pstr = new char[1]();
        _pstr[0] = '\0';
    }else{
        _pstr = new char[strlen(lhs) + 1]();
        memmove(_pstr, lhs, strlen(lhs) + 1);
    }
}

String::String(const String &lhs)
: _pstr(new char[strlen(lhs._pstr) + 1]()){
    cout << "String::String(const String &lhs)" << endl;
    memmove(_pstr, lhs._pstr, strlen(lhs._pstr) + 1);
}

String::~String(){
    cout << "~String()" << endl;
    if(_pstr){
        delete []_pstr;
        _pstr = nullptr;
    }
}

String& String::operator=(const String &lhs){
    cout << "String& String::operator=(const String &lhs)" << endl;
    if(this->_pstr != lhs._pstr){//自复制
        delete []_pstr;
        if(lhs._pstr == nullptr){
            //本写法该情况不会出现
            _pstr = nullptr;
        }else{
            _pstr = new char[strlen(lhs._pstr) + 1];//因为构造时不会为空指针,不用判断
            memmove(_pstr, lhs._pstr, strlen(lhs._pstr) + 1);
        }

    }
    return *this;

}

String& String::operator=(const char *lhs){
    cout << "String& String::operator=(const char *lhs)" << endl;
    if(nullptr == lhs){//string是直接令其为nullptr的
        if(strlen(_pstr)){
            delete []_pstr;
            _pstr = new char[1]();
            _pstr[0] = '\0';
        }
    }else{
        _pstr = new char[strlen(lhs) + 1]();
        strcpy(_pstr, lhs);
    }
    return *this;
}

String& String::operator+=(const String& lhs){
    if(strlen(lhs._pstr)){
        char *tmp = new char[strlen(_pstr) + strlen(lhs._pstr) + 1]();
        memmove(tmp, _pstr, strlen(_pstr));
        memmove(tmp + strlen(_pstr), lhs._pstr, strlen(lhs._pstr) + 1);
        delete[] _pstr;
        _pstr = tmp;
        tmp = nullptr;
    }  
    return *this;
}


String& String::operator+=(const char *lhs){
    if(lhs){
        char *tmp = new char[strlen(_pstr) + strlen(lhs) + 1];
        memmove(tmp, _pstr, strlen(_pstr));
        memmove(tmp + strlen(_pstr), lhs, strlen(lhs) + 1);
        delete[] _pstr;
        _pstr = tmp;
        tmp = nullptr;
    }        
    return *this;
}

char& String::operator[](std::size_t idx){
    if(idx < strlen(_pstr)){
        return _pstr[idx];
    }else{
        return _pstr[strlen(_pstr)];// '\0'
    }
}

const char& String::operator[](std::size_t idx) const{
    cout << "const char& String::operator[](std::size_t idx) const" << endl;
    if(idx < strlen(_pstr)){
        return _pstr[idx];
    }else{
        return _pstr[strlen(_pstr)];// '\0'
    }
} 

std::size_t String::size() const{
    return strlen(_pstr);
}

const char* String::c_str() const{
    return _pstr;
}

bool operator==(const String &lhs, const String &rhs){
    return !strcmp(lhs.c_str(), rhs.c_str());
}


bool operator!=(const String &lhs, const String &rhs){
    return strcmp(lhs.c_str(), rhs.c_str());
}

bool operator<(const String &lhs, const String &rhs){
    if(strcmp(lhs._pstr, rhs._pstr) < 0){
        return true; 
    }
    return false;
}

bool operator>(const String &lhs, const String &rhs){
    if(strcmp(lhs._pstr, rhs._pstr) > 0){
        return true; 
    }
    return false;
}

bool operator<=(const String &lhs, const String &rhs){
    if(strcmp(lhs._pstr, rhs._pstr) <= 0){
        return true; 
    }
    return false;
}

bool operator>=(const String &lhs, const String &rhs){
    if(strcmp(lhs._pstr, rhs._pstr) >= 0){
        return true; 
    }
    return false;
}
std::ostream &operator<<(std::ostream &os, const String &s){
    cout << "std::ostream &operator<<(std::ostream &os, const String &s)" << endl;
    /* cout << s._pstr;//os or cout 不影响 */
    if(strlen(s._pstr)){
        os << s._pstr;
    }else{
        os << "nullptr";
    }
    return os;
}

//#define MAX_INPUT_BUFFER 1024*2, 没有判断流的状态
std::istream &operator>>(std::istream &is, String &s){
    //optimize : 用vector
    
    cout << "std::istream &operator>>(std::istream &is, String &s)" << endl;
    char tmp[MAX_INPUT_BUFFER] = { 0 };
    //方式一:
    is >> tmp;
    
    //方式二:
    /* int i = 0; */
    /* char ch; */
    /* while(is >> std::noskipws >> ch){ */
    /*     if(ch == ' ' || ch == '\n'){ */
    /*         break; */
    /*     } */
    /*     tmp[i] = ch; */
    /*     ++i; */
    /* } */
    /* tmp[i] = '\0'; */

    if(s._pstr){
        //可以不判断, 构造函数已经默认使其不为空了
        delete [] s._pstr;//释放做操作数
    }
    s._pstr = new char[strlen(tmp) + 1];
    strcpy(s._pstr, tmp);
    return is;
}

String operator+(const String &lhs, const String &rhs){
    String tmp(lhs);
    tmp += rhs;
    /* if(strlen(rhs._pstr)){ */
    /*     char *tmp = new char[strlen(rhs._pstr) + strlen(lhs._pstr) + 1](); */
    /*     memmove(tmp, lhs._pstr, strlen(lhs._pstr)); */
    /*     memmove(tmp + strlen(lhs._pstr), rhs._pstr, strlen(rhs._pstr) + 1); */
    /*     return String(tmp); */
    /* } */  
    return String(tmp);
}

String operator+(const String &lhs, const char *rhs){
    if(rhs){
        char *tmp = new char[strlen(lhs._pstr) + strlen(rhs) + 1]();
        memmove(tmp, lhs._pstr, strlen(lhs._pstr));
        memmove(tmp + strlen(lhs._pstr), rhs, strlen(rhs) + 1);
        return String(tmp);
    }
    return String(lhs);
}

String operator+(const char *lhs, const String &rhs){
    if(lhs){
        char *tmp = new char[strlen(lhs) + strlen(rhs._pstr) + 1]();
        memmove(tmp, lhs, strlen(lhs));
        memmove(tmp + strlen(lhs), rhs._pstr, strlen(rhs._pstr) + 1);
        return String(tmp);
    }
    return String(rhs);
}
