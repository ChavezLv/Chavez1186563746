#include<iostream>
#include<log4cpp/OstreamAppender.hh>
#include<log4cpp/Appender.hh>
#include<log4cpp/FileAppender.hh>
#include<log4cpp/RollingFileAppender.hh>
#include<log4cpp/PatternLayout.hh>
#include<log4cpp/Priority.hh>
#include<log4cpp/Category.hh>
//#include<log4cpp/>
//#include<log4cpp/>
using std::cout;
using std::endl;
using namespace log4cpp;
int main(){
    //1,日志布局设置;terminal,fileapp,rollingfileapp;
    PatternLayout *ptn1=new PatternLayout();   
    ptn1->setConversionPattern("%d %c [%p] %m%n");
    PatternLayout *ptn2=new PatternLayout();   
    ptn2->setConversionPattern("%d %c [%p] %m%n");
    PatternLayout *ptn3=new PatternLayout();   
    ptn3->setConversionPattern("%d %c [%p] %m%n");
    //2,设置三个目的地;
    OstreamAppender*pos=new OstreamAppender("console",&cout);
    FileAppender*fileApp=new FileAppender("fileApp","wd.log");
    auto rollingfileApp=new RollingFileAppender(
        "rollingFile",
        "rollingwd.log",
        5*1024,
        9);
    //3,创建日志记录器
    Category& saleDepart=Category::getRoot().getInstance("saleDepart");
    saleDepart.setPriority(Priority::DEBUG);
    saleDepart.addAppender(pos);
    saleDepart.addAppender(fileApp);
    saleDepart.addAppender(rollingfileApp);
    //4,记录日志
    int count=200;
    while(count-->0){
        saleDepart.emerg("this is an emerg msg");
        saleDepart.emerg("this is a fatal msg");
        saleDepart.emerg("this is a crital msg");
        saleDepart.emerg("this is an alert msg");
        saleDepart.emerg("this is an error msg");
        saleDepart.emerg("this is a warn msg");
        saleDepart.emerg("this is an info msg");
        saleDepart.emerg("this is a notice msg");
        saleDepart.emerg("this is a debug msg");
    }
    //5,退出日志系统,Category对象内部自动管理资源分配
    Category::shutdown();
    return 0;
}
