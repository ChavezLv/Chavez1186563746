#include<string.h>
#include<iostream>
#include<string>
using std::cout;
using std::endl;
class String 
{
public:
    String(){
        _pstr=new char[1];
    }
    String(const char *pstr){
       _pstr=new char(strlen(pstr)+1);
        strcpy(_pstr,pstr);
           }
	String(const String &str){
# if 0
           _pstr=new char(str.size()+1);
            strcpy(_pstr,str.c_str());
            if(_pstr){
                delete[]_pstr;
            }
#endif
        _pstr=new char(strlen(str._pstr)+1);
        strcpy(_pstr,str._pstr);
    }
    ~String(){
        if(_pstr){
            delete  []_pstr;
            _pstr=nullptr;
            //cout<<"deconstructor"<<endl;
        }
    }
	friend std::ostream &operator<<(std::ostream &os, const String &s);
	friend std::istream &operator>>(std::istream &is, String &s);
    String &operator=(const String &rhs){
        if(_pstr){
            delete []_pstr;
        }
        _pstr=new char[strlen(rhs._pstr)+1];
        strcpy(_pstr,rhs._pstr);
        return *this;
    }
    String &operator=(const char *pstr){
        if(_pstr){
            delete []_pstr;
        }
        _pstr=new char[strlen(pstr)+1];
        strcpy(_pstr,pstr);
        return *this;

    }

    String &operator+=(const String &rhs){
#if 0
        int size=0;
        if(_pstr){
            size=strlen(this->_pstr);
            delete []_pstr;
        }
        _pstr=new char[strlen(rhs._pstr)+size+1];
        strcat(_pstr,rhs._pstr);
#endif
        if(_pstr){
            int size=strlen(this->_pstr);
            char* tmpStr=new char[size+strlen(rhs._pstr)+1];
            strcpy(tmpStr,_pstr);
            strcat(tmpStr,rhs._pstr);
            delete []_pstr;
            _pstr=tmpStr;
        }else{
            strcpy(_pstr,rhs._pstr);
        }
        return *this;
        
    }
	String &operator+=(const char *pstr){
#if 1
        if(_pstr){
            int size=strlen(this->_pstr);
            char* tmpStr=new char[size+strlen(pstr)+1];
            strcpy(tmpStr,_pstr);
            strcat(tmpStr,pstr);
            delete []_pstr;
            _pstr=tmpStr;
        }else{
            strcpy(_pstr,pstr);
        }
#endif
        return *this;
    }
    char &operator[](std::size_t index){
        cout<<"lvalue"<<endl;
        return _pstr[index];//左值
    }
    const char &operator[](std::size_t index) const{
        cout<<"rvalue"<<endl;
        return _pstr[index];//右值
    }
	
    std::size_t size() const{
        return (size_t)strlen(_pstr);
    }
    const char* c_str() const{
        return _pstr;
    }
    
#if 0    
	
	friend bool operator==(const String &, const String &);
	friend bool operator!=(const String &, const String &);
	
	friend bool operator<(const String &, const String &);
	friend bool operator>(const String &, const String &);
	friend bool operator<=(const String &, const String &);
friend bool operator>=(const String &, const String &);
	
#endif
private:
	char * _pstr;
};
#if 1
String operator+(const String &rhs, const String &lhs){
#if 1
    char* tmpStr=new char[rhs.size()+lhs.size()+1]();
    memset(tmpStr,'\0',strlen(tmpStr));
    strcpy(tmpStr,rhs.c_str());
    strcat(tmpStr,lhs.c_str());
    String str(tmpStr);
    //delete[] tmpStr;
    return str;
#endif

}
String operator+(String rhs, const char *lhs)
{
#if 1
    static char* tmpStr=new char[rhs.size()+strlen(lhs)+1]();
    memset(tmpStr,'\0',strlen(tmpStr));
    strcpy(tmpStr,rhs.c_str());
    strcat(tmpStr,lhs);
    String str(tmpStr);
#endif
    //delete[] tmpStr;
    return str;
}
String operator+(const char *lhs, String rhs){
#if 1
    char* tmpStr=new char[rhs.size()+strlen(lhs)+1]();
    memset(tmpStr,'\0',strlen(tmpStr));
    strcpy(tmpStr,lhs);
    strcat(tmpStr,rhs.c_str());
    String str(tmpStr);
    //delete []tmpStr;
    return str;
#endif
}
#endif
std::ostream &operator<<(std::ostream &os, const String &s){
    os<<s._pstr;
    return os;
}
std::istream &operator>>(std::istream &is, String &s){
    is>>s._pstr;
    return is;
}
