#include "MyLogger.h"
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/Priority.hh>
#include <iostream>

using std::cout;
using std::endl;

Mylogger *Mylogger::_pInstance = nullptr;//饿汉模式，等待进程创建实例。
//多线程环境不友好

Mylogger *Mylogger::getInstance()
{
    if(nullptr == _pInstance)
    {
        _pInstance = new Mylogger();
    }//单例模式：创建唯一实例in堆

    return _pInstance; 
}

void Mylogger::destroy()
{
    if(_pInstance)
    {//析构函数（多线程有竞争风险）
        delete _pInstance;
        _pInstance = nullptr;
    }
}

void Mylogger::warn(const char * msg)
{
    _mycat.warn(msg);
}

void Mylogger::error(const char * msg)
{
    _mycat.error(msg);
}

void Mylogger::debug(const char * msg)
{
    _mycat.debug(msg);
}

void Mylogger::info(const char * msg)
{
    _mycat.info(msg);
}

Mylogger::Mylogger()
: _mycat(Category::getRoot().getInstance("Mycat"))//category格式设置器
{
    //日志的格式
    PatternLayout *ppl1 = new PatternLayout();
    ppl1->setConversionPattern("%d %c [%p] %m %n");

    PatternLayout *ppl2 = new PatternLayout();
    ppl2->setConversionPattern("%d %c [%p] %m %n");

    //日志的目的地
    OstreamAppender *poa = new OstreamAppender("OstreamAppender123", &cout); //输出到终端
    poa->setLayout(ppl1);

    FileAppender *pfa = new FileAppender("FileAppender123", "wd.log");
    pfa->setLayout(ppl2);

    _mycat.addAppender(poa);//把所有属性都附加到格式设置上
    _mycat.addAppender(pfa);
    _mycat.setPriority(Priority::DEBUG);
}

Mylogger::~Mylogger()
{
    Category::shutdown();
}
