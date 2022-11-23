#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include <log4cpp/BasicLayout.hh>
#include <log4cpp/SimpleLayout.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/RollingFileAppender.hh>
#include <log4cpp/Category.hh>
#include <log4cpp/Priority.hh>
/* using namespace std; */
using std::cin;
using std::cout;
using std::endl;
using std::string;
/* using std::vector; */

using namespace log4cpp; //一次性将log4cpp的实体全部引出

class MyLogger {
public:
    static MyLogger* getInstance(string logName = "mylog.txt");
    static void delInstance();
    
    void getMsg(string &msg, const char*msgfrom);
    void warn(const char *msg);
    void error(const char *msg);
    void debug(const char *msg);
    void info(const char *msg);
private:
    MyLogger(string logName);
    ~MyLogger();
private:
    PatternLayout *_pPattLayout;
    RollingFileAppender *_pRollFileApp;
    Category &_root;

    PatternLayout *_pPattLayout2;
    OstreamAppender *_posOs;
    static MyLogger* _pMylog;
};

MyLogger* MyLogger::_pMylog = nullptr;

MyLogger::MyLogger(string logName)
: _pPattLayout(new PatternLayout())
, _pRollFileApp(new RollingFileAppender("?", logName, 5 * 1024, 3))
, _root(Category::getRoot().getInstance(__func__))
/* , _root(Category::getRoot()) */

, _pPattLayout2(new PatternLayout())
, _posOs(new OstreamAppender("?", &cout)){
    operator<<(cout, "MyLogger()")<< endl;
    _root.setPriority(Priority::ERROR);

    _pPattLayout->setConversionPattern("%d %c [%p] %m %n");
    _pRollFileApp->setLayout(_pPattLayout);
    _root.addAppender(_pRollFileApp);


    _pPattLayout2->setConversionPattern("%d %c [%p] %m %n");
    _posOs->setLayout(_pPattLayout2);
    _root.addAppender(_posOs);
}

MyLogger::~MyLogger(){
    operator<<(cout, "~MyLogger()")<< endl;
}

MyLogger* MyLogger::getInstance(string logName){
    if(!_pMylog){
        _pMylog = new MyLogger(logName);
    }
    return _pMylog;
}

void MyLogger::delInstance(){
    if(_pMylog){
        delete _pMylog;
        _pMylog = nullptr;
    }
}

void MyLogger::getMsg(string& msg, const char *msgfrom){
    char line[20];
    string msg1 = __FILE__;
    msg1.append("-");
    msg1.append(__func__);
    msg1.append("-");
    sprintf(line, "%d : ", __LINE__);
    msg1.append(line);

    msg1.append(msgfrom);
    msg = msg1;
}

void MyLogger::warn(const char *msg){
    string pre;
    getMsg(pre, msg);
    _root.warn(pre.c_str());
}

void MyLogger::error(const char *msg){
    string pre;
    getMsg(pre, msg);
    _root.error(pre.c_str());
}

void MyLogger::debug(const char *msg){
    string pre;
    getMsg(pre, msg);
    _root.debug(pre.c_str());
}
void MyLogger::info(const char *msg){
    string pre;
    getMsg(pre, msg);
    _root.fatal(pre.c_str());
}

MyLogger *pIns = MyLogger::getInstance("ttt.tt");
void test(){
    /* MyLogger *pIns = MyLogger::getInstance(); */
    string msg;
    char line[20];
    sprintf(line, "-line(%d) : ", __LINE__);
    for(int i = 0; i < 3; ++i){
        pIns->getMsg(msg, "The log is error message"); 
        msg.append(line); 
        pIns->error(msg.c_str());
    }
    /* MyLogger::delInstance(); */
}
void test2(){
    for(int i = 0; i < 3; ++i){
        pIns->error("The log is error message");
        pIns->info("The log is info message");
        pIns->debug("The log is debug message");
        pIns->warn("The log is warn message");

    }
}
int main()
{
    for(int i = 0; i < 50; ++i){
        test();
        /* test2(); */
    }
    MyLogger::delInstance();
    return 0;
}

