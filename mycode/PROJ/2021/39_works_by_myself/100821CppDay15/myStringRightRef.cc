#include <string.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
/* using namespace std; */
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::ostream;

class String {
    class Char{
    public:
        Char(String &lhs, size_t idx = 0);
        ~Char();
        Char &operator=(const char &ch);//写操作
        Char &operator=(const int &ascii);//const char &ch会自动识别, 这里不需要
        operator char() const{
            cout << "operator char() const" << endl;
            return _str._pstr[_idx];
        }
        const char& c_char(){
            return _str._pstr[_idx];
        }

        operator char() {
            return _str._pstr[_idx];
        }

        friend std::ostream& operator<<(std::ostream &os, const String::Char &rhs);

    private:
        String &_str;
        size_t _idx;
    };
public:
    //无参构造
    String()
    : _pstr(new char[5]() + 4){
        _pstr[0] = '\0';
        cout << "String()" << endl;
        initReferenceCnt();
    }

    //构造: const char *
    String(const char *pstr)
    : _pstr(new char[strlen(pstr) + 5]() + 4){
        cout << "String(const char *pstr)" << endl;
        memmove(_pstr, pstr, strlen(pstr) + 1);
        initReferenceCnt();
    }

    //构造: const String & String(const String &rhs)
    String(const String &lhs)
    : _pstr(lhs._pstr){
        //浅拷贝
        cout << "String(const String &lhs)" << endl;
        addReferenceCnt();
    }

    //移动构造函数
    String(String &&rhs)
    : _pstr(rhs._pstr){
        cout << "String(String &&rhs)" << endl;
        rhs._pstr = nullptr;
    }

    String& operator=(const String &lhs){
        cout << "String& operator=(const String &lhs)" << endl;
        if(this != &lhs){//自复制
            subtractReferenceCnt();
            if(0 == getReferenceCnt()){
                release();
            }     
            _pstr = lhs._pstr;//浅拷贝
            addReferenceCnt();//自复制时, s1 = s1 , 不需要任何操作!
        }
        return *this;
    }

    //移动赋值函数
    String & operator=(String &&rhs){
        cout << "String &operator=(String &&rhs)" << endl;
        if(this != &rhs){
            subtractReferenceCnt();
            if(0 == getReferenceCnt()){
                release();
            }     
            _pstr = rhs._pstr;
            rhs._pstr = nullptr;
        }
        return *this;
    }

    String& operator=(const char *pstr){
        cout << "String& operator=(const char *pstr)" << endl;
        if(0 != strcmp(_pstr, pstr)){
            *this = String(pstr);
        }
        return *this;
    }

    // Char = 'h'
    Char operator[](int idx){
        return Char(*this, idx);
    }

    void release(){
        delete [](_pstr - 4);
        _pstr = nullptr;

    }

    ~String(){
        cout << "~String()" << endl;
        if(_pstr){// 如果没有判断这个, 在移动赋值时为临时对象的成员nullptr\
            此时时无法getReferenceCnt()的, 会core dumped
            if(getReferenceCnt() > 0){
                subtractReferenceCnt();
                if(0 == getReferenceCnt()){
                    release();

                }
            }
        }            
    }

    int getReferenceCnt()const{
        return *(int *)(_pstr - 4);
    }
    void initReferenceCnt()const{
        *(int *)(_pstr - 4) = 1;

    }

    void addReferenceCnt()const{
        ++ *(int *)(_pstr - 4);
    }

    void subtractReferenceCnt()const{
        -- *(int *)(_pstr - 4);
    }


    const char* c_str(){
        return _pstr;
    }

    friend std::ostream &operator<<(std::ostream &os, const String &rhs);
    friend std::ostream& operator<<(std::ostream &os, const Char &rhs);

private:
    char *_pstr;

};

//Class Char
String::Char::Char(String &lhs, size_t idx)
: _str(lhs)
, _idx(idx){
    cout << "Char()" << endl;
}

//Class Char
String::Char::~Char(){
    cout << "~Char" << endl;
}


//Class Char
// String[idx] = ch -->  Char = ch
String::Char &String::Char::operator=(const char &ch){
    cout << "String::Char &String::Char::operator=(const char &ch)" << endl;
    if(_str._pstr[_idx] != ch){
        /* if(_str.getReferenceCnt() > 1){ */
            /* _str.subtractReferenceCnt(); //不要写这个了!*/
            
            _str = String(_str._pstr);
        /* } */
        _str._pstr[_idx] = ch;

    }  
    return *this;
}

String::Char &String::Char::operator=(const int &ascii){
    cout << "String::Char &String::Char::operator=(const int &ascii)" << endl;
    if(ascii >= 0 && ascii <= 127){
        _str = String(_str._pstr);
        _str._pstr[_idx] = ascii;
    }
    return *this;
}

std::ostream& operator<<(std::ostream &os, const String::Char &rhs){
    cout << "std::ostream& operator<<(std::ostream &os, const String::Char &rhs)" << endl;
    if(rhs._str._pstr){
        os << rhs._str.c_str()[rhs._idx];
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, const String &rhs){
    if(rhs._pstr){
        os << rhs._pstr;
    }
    return os;
} 

void test(){
    String s1("hello");
    s1 = "world";
    printf("s1:%s, %p, %d\n", s1.c_str(), s1.c_str(), s1.getReferenceCnt());
    //当没有重载 = 时:
    //String(const char *pstr)
    //String& operator=(const String &rhs)
    //
    //重载之后:
    //String& operator=(const char *pstr)
    //String(const char *pstr)
    //String& operator=(const String &lhs)
}


void test1(){
    /* String s1("world"); */
    String s1("hello");
    /* s2 = s1; */
    String s2(s1);

    s2 = "world";
    s2[0] = 'W';

    printf("s1:%s, %p, %d\n", s1.c_str(), s1.c_str(), s1.getReferenceCnt());
    printf("s2:%s, %p, %d\n", s2.c_str(), s2.c_str(), s2.getReferenceCnt());
    /* printf("s3:%s, %p, %d\n", s3.c_str(), s3.c_str(), s3.getReferenceCnt()); */

}

void test2(){
    String s1("hello");
    String s2(s1);

    printf("s1:%s, %p, %d\n", s1.c_str(), s1.c_str(), s1.getReferenceCnt());
    printf("s2:%s, %p, %d\n", s2.c_str(), s2.c_str(), s2.getReferenceCnt());

    s2[0] = 'H'; 
    s2[1] = 97;
    char ch = s2[0].c_char();
    s2[3];

    printf("s1:%s, %p, %d\n", s1.c_str(), s1.c_str(), s1.getReferenceCnt());
    printf("s2:%s, %p, %d\n", s2.c_str(), s2.c_str(), s2.getReferenceCnt());
}

void test3(){
    String s1("hello");
    String s2(s1);

    printf("s1:%s, %p, %d\n", s1.c_str(), s1.c_str(), s1.getReferenceCnt());
    printf("s2:%s, %p, %d\n", s2.c_str(), s2.c_str(), s2.getReferenceCnt());

    s1[0];
    printf("s1:%s, %p, %d\n", s1.c_str(), s1.c_str(), s1.getReferenceCnt());
    printf("s2:%s, %p, %d\n", s2.c_str(), s2.c_str(), s2.getReferenceCnt());

    //String[idx] -->  Char
    cout << s2[1] << endl;
    printf("s1:%s, %p, %d\n", s1.c_str(), s1.c_str(), s1.getReferenceCnt());
    printf("s2:%s, %p, %d\n", s2.c_str(), s2.c_str(), s2.getReferenceCnt());
}

void test4(){
    String s1(String("hello"));
    printf("s1:%s, %p, %d\n", s1.c_str(), s1.c_str(), s1.getReferenceCnt());
    cout << s1 << endl;

    s1 = String("world");
    cout << s1 << endl;
}

int main()
{
    operator<<(cout, "new test") << endl << endl;
    /* test(); */
    operator<<(cout, "new test") << endl << endl;
    /* test1(); */
    operator<<(cout, "new test") << endl << endl;
    /* test2(); */
    operator<<(cout, "new test") << endl << endl;
    /* test3(); */
    operator<<(cout, "new test") << endl << endl;
    test4();

    return 0;
}

