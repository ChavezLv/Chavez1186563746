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

    /* void warn(const char *msg); */
    template<typename ... Args>
    void warn(const char *msg, Args ... args){
        _root.warn(msg, args ...);
    }

    /* void error(const char *msg); */
    template<typename ... Args>
        void error(const char *msg, Args ... args){
            _root.error(msg, args ...);
        }

    /* void debug(const char *msg); */
    template<typename ... Args>
        void debug(const char *msg, Args ... args){
            _root.debug(msg, args...);
        }

    // 模板的实现和声明不要分开! 和内联一样的, 头文件中,包含声明
    template<typename ... Args>
        void info(const char *msg, Args ... args){
            _root.info(msg, args...);
        }

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
#define suffix(msg, ...) (string(__FILE__) + string("/")\
    + string(__func__) + string("/")\
    + string(std::to_string(__LINE__)) + string("/") + msg).c_str() 

#define  logError(msg, ...) (plog->error(suffix(msg), ##__VA_ARGS__));
/* #define logError(msg, ...) {\ */
/*     char tmp[510];\ */
/*     sprintf(tmp, msg, ##__VA_ARGS__);\ */
/*     plog->error(suffix(tmp));\ */
/*     } */

#define  logWarn(msg, ...) (plog->warn(suffix(msg), ##__VA_ARGS__));
/* #define logWarn(msg, ...) {\ */
/*     char tmp[510];\ */
/*     sprintf(tmp, msg, ##__VA_ARGS__);\ */
/*     plog->warn(suffix(tmp));\ */
/*     } */

//多参数传参方式一: 
#define  logInfo(msg, ...) (plog->info(suffix(msg), ##__VA_ARGS__));
//多参数传参方式二: 
/* #define logInfo(msg, ...) {\ */
/*     char tmp[510];\ */
/*     sprintf(tmp, msg, ##__VA_ARGS__);\ */
/*     plog->info(suffix(tmp));\ */
/*     } */

#define  logDebug(msg, ...) (plog->debug(suffix(msg), ##__VA_ARGS__));
/* #define logDebug(msg, ...) {\ */
/*     char tmp[510];\ */
/*     sprintf(tmp, msg, ##__VA_ARGS__);\ */
/*     plog->debug(suffix(tmp));\ */
/*     } */

/* #define logInfo(msg) (plog->info(suffix(msg))) */ 
/* #define logWarn(msg) (plog->warn(suffix(msg))) */ 
/* #define logDebug(msg) (plog->debug((suffix(msg)))) */ 

#endif
/* #include "../src/mylogTemplate.cc" */
