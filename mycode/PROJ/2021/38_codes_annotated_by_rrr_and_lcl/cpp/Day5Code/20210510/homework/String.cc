#include <string.h>
#include <iostream>


using std::cout;
using std::endl;

class String
{
public:
    String()
    : _pstr(nullptr)
    /* : _pstr(new char[1]())//ok */
    {
        cout << "String()" << endl;
    }

    //pstr = nullptr
    String(const char *pstr)
    : _pstr(new char[strlen(pstr) + 1]())
    {
        cout << "String(const char *)" << endl;
        strcpy(_pstr, pstr);
    }

    String(const String & rhs)
    : _pstr(new char[strlen(rhs._pstr) + 1]())
    {
        cout << "String(const char *)" << endl;
        strcpy(_pstr, rhs._pstr);

    }

    //str1 = str2;
    String & operator=(const String & rhs)
    {
        cout << "String &operator=(const String &)" << endl;
        if(this != &rhs)//1、自复制
        {
            if(_pstr)
            {
               delete [] _pstr;//2、释放左操作数
               _pstr = nullptr;
            }

            _pstr = new char[strlen(rhs._pstr) + 1]();//3、深拷贝
            strcpy(_pstr, rhs._pstr);
        }

        return *this;//4、返回*this
    }

    ~String()
    {
        cout << "~String()" << endl;
        if(_pstr)
        {
            delete [] _pstr;
            _pstr = nullptr;
        }
    }

    void print() const
    {
        if(_pstr)
        {
            cout << _pstr << endl;
        }
    }

private:
    char * _pstr;

};

int main(void)
{
    String str1;
    str1.print();
#if 1
    String str2 = "Hello,world";
    String str3("wangdao");

    str2.print();       
    str3.print();   

    String str4 = str3;
    str4.print();

    str4 = str2;
    str4.print();
#endif
    return 0;

}
