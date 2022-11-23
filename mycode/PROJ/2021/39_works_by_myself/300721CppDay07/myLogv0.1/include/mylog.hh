#ifndef __MYLOGGER_H__
#define __MYLOGGER_H__
#include <log4cpp/Category.hh>
/* #include <log4cpp/BasicLayout.hh> */
/* #include <log4cpp/SimpleLayout.hh> */
/* #include <log4cpp/PatternLayout.hh> */
/* #include <log4cpp/OstreamAppender.hh> */
/* #include <log4cpp/FileAppender.hh> */
/* #include <log4cpp/RollingFileAppender.hh> */
/* #include <log4cpp/Priority.hh> */

using std::string;

using namespace log4cpp; //一次性将log4cpp的实体全部引出

class MyLogger {
private:
    /* class AutoRelease{ */
    /* public: */
    /*     ~AutoRelease(); */
    /* }; */
public:
    static MyLogger* getInstance(string logName = "mylog.txt");
    static void rmInstance();

    void warn(const char *msg);
    void error(const char *msg);
    void debug(const char *msg);
    void info(const char *msg);
private:
    MyLogger(string logName = "mylog.txt");
    ~MyLogger();
private:
    static MyLogger* _pMylog;
    /* static AutoRelease _auto; */
    Category &_root;//log里面除了这个,其他没有必要!

    /* PatternLayout *_pPattLayout; */
    /* RollingFileAppender *_pRollFileApp; */

    /* PatternLayout *_pPattLayout2; */
    /* OstreamAppender *_posOs; */

};
#endif
