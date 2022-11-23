#include"logger.hpp"
#include<iostream>
#include<log4cpp/Priority.hh>
#include<log4cpp/PatternLayout.hh>
#include<log4cpp/OstreamAppender.hh>
#include<log4cpp/Appender.hh>
#include<log4cpp/FileAppender.hh>
using std::cout;
using std::endl;
using std::string;
using namespace log4cpp;
namespace wd{
    MyLog* MyLog::_pInstance=nullptr;
    MyLog* MyLog::getInstance(){
        if(_pInstance==nullptr){
            _pInstance=new MyLog();
        }
        return _pInstance;
    }
    void MyLog::destroy(){
        if(_pInstance!=nullptr){
            delete _pInstance;
            _pInstance=nullptr;
        }
    }
    MyLog::MyLog()
        :_mycat(Category::getRoot().getInstance("mymodule"))
    {
        auto ptn1=new PatternLayout();
        ptn1->setConversionPattern("%d %c [%p] %m%n");
        auto ptn2=new PatternLayout();
        ptn2->setConversionPattern("%d %c [%p] %m%n");
        auto pos=new OstreamAppender("consolve",&cout);
        auto pfile=new FileAppender("fileApp","wd.log");
        pos->setLayout(ptn1);
        pfile->setLayout(ptn2);

        _mycat.setPriority(Priority::DEBUG);
        _mycat.addAppender(pos);
        _mycat.addAppender(pfile);
        cout<<"mylog has finish the initialision"<<endl;
    }
    MyLog::~MyLog(){
        Category::shutdown();
        cout<<"~MyLog()"<<endl;
    }
}
