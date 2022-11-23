// <Mylog> -*- C++ -*-
#ifndef __MYLOG_H__
#define __MYLOG_H__
#include <log4cpp/Category.hh>
#include <log4cpp/config.h>
#include <string>
#include <sstream>

// 定义日志文件的名称
#define LOGFILENAME "../log/Mylog.log"

enum Priority{
    EMERG = 0, FATAL = 0, ALERT = 100, CRIT = 200,
    ERROR = 300, WARN = 400, NOTICE = 500, INFO = 600,
    DEBUG = 700, NOTSET = 800
};

class Mylog
{
public:
    static Mylog & getInstance();
    static void destroy();

    void setPriority(Priority = DEBUG);
    template <class T, class... Args>
    void log(Priority,
             const char * file,
             const char * func,
             int line,
             T,
             Args...);

private:
    Mylog(const char * = LOGFILENAME);

    static Mylog * _log;
    log4cpp::Category & _category;
};

inline std::string int2string(int line)
{
    std::ostringstream oss;
    oss << line;
    return oss.str();
}


template <class Tmsg>
void strMerge(std::ostringstream & oss, Tmsg & msg)
{
    oss << msg;
}

template <class Tmsg, class... Args>
void strMerge(std::ostringstream & oss, Tmsg & msg, Args &... args)
{
    oss << msg;
    strMerge(oss, args...);
}

template <class T, class... Args>
void Mylog::log(Priority pri,
                const char * file,
                const char * func,
                int line,
                T mag,
                Args... args)
{
    std::string logtext;
    std::ostringstream oss;
    strMerge(oss, mag, args...);
    logtext = oss.str();
    // Info 不输出文件名、函数名、行数
    if(pri != Priority::INFO) {
        logtext.append("   --> FILE: ")
            .append(file)
            .append(" FUNC: ")
            .append(func)
            .append(" LINE: ")
            .append(int2string(line));
    }
    _category.log(pri, logtext);
}

// 不需要主函数中定义logCpp
// 主函数中定义 __LOG4CPP__
#ifdef __LOG4CPP__
Mylog & logCpp = Mylog::getInstance();
#else
extern Mylog & logCpp;
#endif

// 输出加上文件名字、函数名字以及行号，这些是在预处理时进行替换的
#define logError(msg, args...) logCpp.log(Priority::ERROR, __FILE__, __func__, __LINE__, msg, ##args)
#define logWarn(msg, args...) logCpp.log(Priority::WARN, __FILE__, __func__, __LINE__, msg, ##args)
#define logInfo(msg, args...) logCpp.log(Priority::INFO, __FILE__, __func__, __LINE__, msg, ##args)
#define logDebug(msg, args...) logCpp.log(Priority::DEBUG, __FILE__, __func__, __LINE__, msg, ##args)

#endif

