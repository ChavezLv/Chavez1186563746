#define __LOG4CPP__ //注意需要在第一行引入,为什么? 这里要放在第一行, 因为在头文件中
//因为这里引入这个宏定义的下一行就是 引入带有该宏定义的头文件, 假如没有在这之前引入,
//就会导致头文件运行导#ifdef __LOG4CPP__时看不到这个, 就会出错!
#include "../include/mylog.hh"
#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::string;

//文件名 __FILE__ ,函数名 __func__ ，行号__LINE__ 是编译器实现的
/* #define suffix(msg) (string(__FILE__) + string("/")\ */
/*     + string(__func__) + string("/")\ */
/*     + string(std::to_string(__LINE__)) + string("/") + msg).c_str() */

/* #define logError(msg) (MyLogger::getInstance()->error(suffix(msg))) */ 
/* #define logInfo(msg) (MyLogger::getInstance()->info(suffix(msg))) */ 
/* #define logWarn(msg) (MyLogger::getInstance()->warn(suffix(msg))) */ 
/* #define logDebug(msg) (MyLogger::getInstance()->debug((suffix(msg)))) */ 

void test(){
    MyLogger *p1 = MyLogger::getInstance();
    MyLogger *p2 = MyLogger::getInstance();
    cout << p1 << endl;
    cout << p2 << endl;

    /* logError("msg"); */
    /* logError("msg"); */
    /* logError("msg"); */
    /* logError("msg"); */
}
int main()
{
    test();
    return 0;
}

