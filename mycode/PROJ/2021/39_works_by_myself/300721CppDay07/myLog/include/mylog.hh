#ifndef __MYLOGGER_H__
#define __MYLOGGER_H__

#include <log4cpp/Category.hh>

using std::string;

using namespace log4cpp; //一次性将log4cpp的实体全部引出


class MyLogger {
private:
    class AutoRelease{
    public:
        ~AutoRelease();
    };
public:
    static MyLogger* getInstance(string logName = "mylog.txt");

    void warn(const char *msg);
    void error(const char *msg);
    void debug(const char *msg);
    void info(const char *msg);

private:
    MyLogger(string logName = "mylog.txt");
    ~MyLogger();

private:
    static MyLogger* _pMylog;
    static AutoRelease _auto;
    Category &_root;
};

////不用每次使用时写 getInstance语句
////只需要在主函数文件中写: #define _LOG4CPP_即可在整个程序中使用
#ifdef __LOG4CPP__
MyLogger *plog = MyLogger::getInstance();//class要写在这个的前面!包括getInstance()
#else//"#"不要漏掉
extern MyLogger *plog;//引用不在同一个文件中的变量(须为全局)或者函数
#endif

//文件名 __FILE__ ,函数名 __func__ ，行号__LINE__ 是编译器实现的
#define suffix(msg) (string(__FILE__) + string("/")\
    + string(__func__) + string("/")\
    + string(std::to_string(__LINE__)) + string("/") + msg).c_str()

#define logError(msg) (plog->error(suffix(msg))) 
#define logInfo(msg) (plog->info(suffix(msg))) 
#define logWarn(msg) (plog->warn(suffix(msg))) 
#define logDebug(msg) (plog->debug((suffix(msg)))) 

#endif
