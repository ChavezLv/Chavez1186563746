#include "Mylog.hh"

#include <cstdarg>
#include <iostream>
#include <string>
#include <sstream>

#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/RollingFileAppender.hh>
#include <log4cpp/Category.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/Priority.hh>

Mylog * Mylog::_log = nullptr;

Mylog::Mylog(const char * logFileName)
    :_category(log4cpp::Category::getRoot())
{
    log4cpp::PatternLayout * pLayout_os =
        new log4cpp::PatternLayout();
    // 不太理解DNC的作用，不过还是加上了
    pLayout_os->setConversionPattern("%d [%-6p] %x : %m%n");

    log4cpp::PatternLayout * pLayout_rf =
        new log4cpp::PatternLayout();
    // layout不可以重复使用
    pLayout_rf->setConversionPattern("%d [%-6p] %x : %m%n");

    log4cpp::OstreamAppender * osAppender =
        new log4cpp::OstreamAppender("osApp", &std::cout);
    osAppender->setLayout(pLayout_os);

    log4cpp::RollingFileAppender * rfAppender =
        new log4cpp::RollingFileAppender("rfApp",
                                         logFileName,
                                         10 * 1024,
                                         3);
    rfAppender->setLayout(pLayout_rf);

    _category.setPriority(log4cpp::Priority::DEBUG);
    _category.addAppender(osAppender);
    _category.addAppender(rfAppender);

    _category.info("start log");
}

Mylog & Mylog::getInstance()
{
    if(nullptr == _log)
    {
        _log = new Mylog();
    }
    return *_log;
}

void Mylog::destroy()
{
    if(_log)
    {
        _log->_category.info("destroy log");
        _log->_category.shutdown();
        delete _log;
        _log = nullptr;
    }
}

void Mylog::setPriority(Priority pri)
{
    _category.setPriority(pri);
}



