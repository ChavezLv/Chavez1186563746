#include <string.h>
#include <iostream>

using std::cout;
using std::endl;

class String
{
public:
    //String s1;
    String()
    : _pstr(new char[5]() + 4)
    {
        cout << "String()" << endl;
       *(int *)(_pstr - 4) = 1;
    }

    //String s1("hello");
    String(const char *pstr)
    : _pstr(new char[strlen(pstr) + 5]() + 4)
    {
        cout << "String(const char *)" << endl;
        strcpy(_pstr, pstr);
        *(int *)(_pstr - 4) = 1;
    }

    //String s2 = s1
    String(const String &rhs)
    : _pstr(rhs._pstr)
    {
        cout << "String(const String &)" << endl;
        ++*(int *)(_pstr - 4);
    }

    //String s3("world");
    //s3 = s1;
    String &operator=(const String &rhs)
    {
        cout << "String &operator=(const String &)" << endl;

        if(this != &rhs)//1、自复制
        {
            --*(int *)(_pstr - 4);//2、释放左操作数
            if(0 == *(int *)(_pstr - 4))
            {
                delete [] (_pstr - 4);
            }

            _pstr = rhs._pstr;//3、浅拷贝
            ++*(int *)(_pstr - 4);
        }

        return *this;//4、返回*this
    }

    //s3[0] = 'H';
    char &operator[](size_t idx)
    {
        if(idx < size())
        {
            if(getRefCount() > 1)//是共享
            {
                char *ptmp = new char[strlen(_pstr) + 5]() + 4;
                strcpy(ptmp, _pstr);
                --*(int *)(_pstr - 4);
                _pstr = ptmp;
                *(int *)(_pstr - 4) = 1;
            }
            return _pstr[idx];
        }
        else
        {
            static char nullchar = '\0';
            return nullchar;
        }
    }

    ~String()
    {
        cout << "~String()" << endl;
        --*(int *)(_pstr - 4);
        if(0 == *(int *)(_pstr - 4))
        {
            delete [] (_pstr - 4);
        }
    }

    const char *c_str() const
    {
        return _pstr;
    }

    int getRefCount() const
    {
        return *(int *)(_pstr - 4);
    }

    size_t size() const
    {
        return strlen(_pstr);
    }

    friend std::ostream &operator<<(std::ostream &os, const String &rhs);
private:
    char *_pstr;
};

std::ostream &operator<<(std::ostream &os, const String &rhs)
{
    if(rhs._pstr)
    {
        os << rhs._pstr;
    }
    return os;
}

void test()
{
    String s1("hello");
    cout << "s1 = " << s1 << endl;
    printf("s1's address: %p\n", s1.c_str());
    printf("s1's RefCount = %d\n", s1.getRefCount());

    cout << endl;
    String s2 = s1;
    cout << "s1 = " << s1 << endl;
    cout << "s2 = " << s2 << endl;
    printf("s1's address: %p\n", s1.c_str());
    printf("s2's address: %p\n", s2.c_str());
    printf("s1's RefCount = %d\n", s1.getRefCount());
    printf("s2's RefCount = %d\n", s2.getRefCount());

    cout << endl;
    String s3("world");
    cout << "s3 = " << s3 << endl;
    printf("s3's address: %p\n", s3.c_str());
    printf("s3's RefCount = %d\n", s3.getRefCount());

    cout << endl << "执行s3 = s1操作" << endl;
    s3 = s1;
    cout << "s1 = " << s1 << endl;
    cout << "s2 = " << s2 << endl;
    cout << "s3 = " << s3 << endl;
    printf("s1's address: %p\n", s1.c_str());
    printf("s2's address: %p\n", s2.c_str());
    printf("s3's address: %p\n", s3.c_str());
    printf("s1's RefCount = %d\n", s1.getRefCount());
    printf("s2's RefCount = %d\n", s2.getRefCount());
    printf("s3's RefCount = %d\n", s3.getRefCount());

    cout << endl << "对s3[0] = \'H\'" << endl;
    s3[0] = 'H';
    cout << "s1 = " << s1 << endl;
    cout << "s2 = " << s2 << endl;
    cout << "s3 = " << s3 << endl;
    printf("s1's address: %p\n", s1.c_str());
    printf("s2's address: %p\n", s2.c_str());
    printf("s3's address: %p\n", s3.c_str());
    printf("s1's RefCount = %d\n", s1.getRefCount());
    printf("s2's RefCount = %d\n", s2.getRefCount());
    printf("s3's RefCount = %d\n", s3.getRefCount());
}

int main(int argc, char **argv)
{
    test();
    return 0;
}

