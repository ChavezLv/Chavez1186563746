#include <string.h>
#include <iostream>

using std::cout;
using std::endl;

//int arr[10] = {1, 3, 5, 7, 9};
//arr.operator[](idx)

class CharArray
{
public:
    CharArray(size_t sz = 10)
    : _size(sz)
    , _data(new char[_size]())
    {
        cout <<"CharArray(size_t = 10)" << endl;
    }

    //函数的返回类型什么时候需要加引用符号？
    //当实体的声明周期大于函数的生命周期的时候，使用引用，如果不使用引用
    //就会在执行return语句的时候，执行拷贝操作（特别是的对于类类型的时候
    //会执行拷贝构造函数，效率上就会降低）
    //
    //重载下标访问运算符能降低越界的风险?
    //
    //下标访问运算符的参数可以是任意类型,例如：std::map
    char &operator[](size_t idx)
    {
        /* cout << "char &operator[](size_t)" << endl; */
        if(idx < _size)
        {
            return _data[idx];
        }
        else
        {
            static char  nullchar = '\0';
            return nullchar;
        }
    }

    size_t size() const
    {
        return _size;
    }

    ~CharArray()
    {
        cout << "~CharArray()" << endl;
        if(_data)
        {
            delete [] _data;
            _data = nullptr;
        }
    }
private:
    size_t _size;;
    char *_data;
};
int main(int argc, char **argv)
{
    const char *pstr = "helloworld";
    CharArray ca(strlen(pstr) + 1);
    for(size_t idx = 0; idx != ca.size();++idx)
    {
        ca.operator[](idx) = pstr[idx];
        ca[idx] = pstr[idx];
        /* 'h' = 'h'; */
    }

    for(size_t idx = 0; idx != ca.size();++idx)
    {
        cout << ca[idx] << "  ";
    }
    cout << endl;


    return 0;
}

