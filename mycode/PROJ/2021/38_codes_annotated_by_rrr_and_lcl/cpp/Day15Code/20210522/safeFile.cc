#include <iostream>

using std::cout;
using std::endl;

class SafeFile
{
public:
    SafeFile(FILE *fp)
    : _fp(fp)
    {
        cout << "SafeFile(FILE *)" << endl;
        if(nullptr == _fp)
        {
            cout << "_fp == nullptr" << endl;
        }
    }

    void write(const string &rhs){
        fwrite(rhs.c_str(), 1, rhs.size());
    }

    ~SafeFile()
    {
        cout << "~SafeFile()" << endl;
        if(_fp)
        {
            fclose(_fp);//这个在c里面是个难题,但是c++这里直接放到析构函数就行了
            // 这个就是后面要学的RAII
            // 比如要求, 不允许赋值或复制, 则可以private或者 = delete;(c++11里面提供的)
            cout << "fclose(_fp)" << endl;
        }
    }
private:
    FILE *_fp;
};
int main(int argc, char **argv)
{
    SafeFile sf(fopen("wuhan.txt", "a+"));//栈对象
    return 0;
}

