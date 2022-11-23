#include "../include/mylog.hh"
#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::string;

//文件名 __FILE__ ,函数名 __func__ ，行号__LINE__ 是编译器实现的
#define suffix(msg) (string(__FILE__) + string("/")\
    + string(__func__) + string("/")\
    + string(std::to_string(__LINE__)) + string("/") + msg).c_str()

#define logError(msg) (MyLogger::getInstance()->error(suffix(msg))) //外层大括号其实没必要
#define logInfo(msg) (MyLogger::getInstance()->info(suffix(msg))) //外层大括号其实没必要
#define logWarn(msg) (MyLogger::getInstance()->warn(suffix(msg))) //外层大括号其实没必要
#define logDebug(msg) (MyLogger::getInstance()->debug((suffix(msg)))) //外层大括号其实没必要

void test(){
    logError("msg");
    logError("msg");
    logError("msg");
    logError("msg");
    MyLogger::rmInstance();
    MyLogger::rmInstance();
    MyLogger::rmInstance();
    MyLogger::rmInstance();
}
int main()
{
    test();
    return 0;
}

