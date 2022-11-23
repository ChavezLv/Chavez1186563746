#include <iostream>
#include <memory>
#include <string>

using std::cout;
using std::endl;
using std::unique_ptr;
using std::shared_ptr;
using std::string;

struct FILEClose
{
    void operator()(FILE *fp) //重载删除器
    {
        cout << "fclose()" << endl;
        fclose(fp);
    }
};
// 笔记by lcl:\
// test() //对于unique_ptr\
from fopen -> fwrite  没有关掉, 是写到文件缓冲区里面去的\
fclose(up.get()); //刷新缓冲区, 可以写入了, 但是出现了double free的问题\
原因: 默认情况下有一个删除器, 传入new的时候用的时delete, 但是这里\
        用的时 FILE, 肯定不能用delete; 默认删除器不够用\
解决: 重载函数调用运算符, 重新改写void operator()(FILE *fp){}\
比如: 之前词频统计的比较, 要重载一下才能比较

//test2() //对于shared_ptr\
没有关掉同样没写入, 手写fclose, 同样会core_dumped, 但是shared_ptr没有\
模板参数deleter.  不过有特定构造函数可以多传一个小对象\
从下面代码可知, 可以传左值, 可以传右值, 无区别


void test()
{
    string msg = "hello,world\n";
    unique_ptr<FILE, FILEClose> up(fopen("wuhan.txt", "a+")); 
    //unique类的删除器基于类+重载【()】运算符，且填入参数【模板类的第二个】
    fwrite(msg.c_str(), 1, msg.size(), up.get());
    /* fclose(up.get()); */
}


void test2()
{
    /* FILEClose fc; */
    string msg = "hello,world\n";
    /* shared_ptr<FILE> sp(fopen("wd.txt", "a+"), fc); */
    shared_ptr<FILE> sp(fopen("wd.txt", "a+"), FILEClose());
    //shared类的删除器基于重载【()】运算符+函数对象，且参数填在【构造函数第2个，填对象】
    fwrite(msg.c_str(), 1, msg.size(), sp.get());
}

int main(int argc, char **argv)
{
    test2();
    return 0;
}

