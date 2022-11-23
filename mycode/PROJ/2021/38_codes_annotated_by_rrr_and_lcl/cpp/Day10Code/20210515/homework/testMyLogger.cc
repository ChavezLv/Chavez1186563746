#include "MyLogger.h"
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

string func(const char *msg)
{
    string s1 = string(__FILE__) + string(" ") + string(__FUNCTION__) 
        + string(" ") + string(std::to_string(__LINE__)) + string(" ") 
        +msg;
    return s1;
}
void test()
{
    /* Mylogger *ps1 = Mylogger::getInstance(); */
    /* ps1->error("hshsh"); */
    /* Mylogger::getInstance()->error("hahha"); */
    /* Mylogger::getInstance()->error(func("hahha").c_str()); */
    /* Mylogger::getInstance()->error(prefix("helloworld")); */
    /* Mylogger::getInstance()->warn(prefix("helloworld")); */
    /* Mylogger::getInstance()->info(prefix("helloworld")); */
    /* Mylogger::getInstance()->debug(prefix("helloworld")); */
    LogError("wuhan");//记录msg为wuhan的错误（其他信息靠它自己填写）
    const char *pstr = "hello";
    int number = 1;
    int value = 2;
    LogError("hello, %d, %s %d\n", number, pstr, value);//可以改机的部分：msg的输入可以改成和printf类似的形式
    //接住省略号...可变参数宏来做（感觉目前看到它都是配合格式输入做的）

}

void test2()//测试定位宏的
{
    cout << __FILE__  << "   " << __FUNCTION__ << "  " << __LINE__ << endl;
}
int main(int argc, char **argv)
{
    test();
    return 0;
}

