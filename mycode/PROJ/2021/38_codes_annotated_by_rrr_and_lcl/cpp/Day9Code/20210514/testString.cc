#include <iostream>

using std::cout;
using std::endl;

//1、全局静态不行
//2、栈不行, const String &rhs,const是常量
//3、文字常量区  程序代码 只读段, 引用计数会进行修改，也不行
//4、只能放在堆上，其实在堆上任何位置都可以，只是为了好获取
//将将引用计数与数据放在一起，并且最好在数据前面，这样当数据
//变化的时候不会移动引用计数的位置
class String
{
public:
    String(const String &rhs)
    : _pstr(rhs._pstr)
    , _cnt(rhs._cnt)
    {
        rhs._cnt++;
    }
private:
    char *_pstr;
    int _cnt;
};
int main(int argc, char **argv)
{
    String s1("Hello");
    String s3 = s1;
    String s2("world")
    return 0;
}

