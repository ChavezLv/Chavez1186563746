#ifndef __MY_LOG_HPP__
#define __MY_LOG_HPP__
/*一,日志系统是将用户指定信息,按照一定的格式,存放到某一个指定区域;
  二,根据面向对象的思想,抽象出一个执行日志记录的主题Category,用于执行日志的开始,结束,记录;
  三,如何执行日志记录,此功能也可以单独抽象出一个类,作为Category的一种属性;

  四,log4cpp的开发主要使用以下四个工具类:
    1,Category类:执行日志记录任务的主体,其构造函数是protect类型,默认有一个root实例,
      其它Category实例均从root衍生出来;使用log4cpp必须有一个Category实例
    2,Appender类:用于描述日志如何输出,例如使用声明格式,输出到哪里(由Layout实现).
      Appender是一个积累,常用的由FileAppender,OstreamAppender,RollingFileAppender;
    3,Layout类:用于描述日志输出的格式;
    4,Priority:用于过滤日志信息.priority根据设置的优先级的不同,会过滤掉相应的优先级(数值越低优先级越高,优先级定义在一个枚举类型中)
 */
#include<string>
#include<log4cpp/Category.hh>
namespace wd{
    //static可以修饰局部变量,函数,全局变量;
    //全局变量存放在静态存储去,位置固定;局部变量存放在栈空间,位置随机;
    //自由存储区,malloc分配的空间,与堆相似(文件共享区)
    //全局存储区(静态存储区):全局变量和静态变量的存储是放在一块的;又划分为已初始化和未初始化;
    //常量存储区存放常量(literal),不允许修改
    
    //普通成员函数占用对象的内存;
    //根据静态成员的特性(没有this指针),静态成员函数只能访问静态成员(静态变量和静态成员函数)
    //因为静态成员函数不用创建实例即可调用,所以对应的只能访问静态成员变量
    //也正因如此,complier不会为static function提供this指针
class MyLog{
public:
    static MyLog* getInstance();
    static void destroy();
    void warn(const char* msg);
    void debug(const char* msg);
    void info(const char* msg);
    void error(const char* msg);
private:
    MyLog();
    ~MyLog();
    static MyLog* _pInstance;
    log4cpp::Category & _mycat;//定义为引用类型,使用Mylog构造函数初始化
    };
}
//获取文件名,当前函数名,当前行数.必须使用宏定义才能返回调用该函数的函数信息;inline都不行;
#define addprefix(msg)  string("[").append(__FILE__)                    \
                        .append(":").append(__func__)                   \
                        .append(":").append(std::to_string(__LINE__))   \
                        .append("]").append(msg).c_str()

#define LogWarn(msg) wd::Mylogger::getInstance()->warn(addprefix(msg))
#define LogError(msg) wd::Mylogger::getInstance()->error(addprefix(msg))
#define LogInfo(msg) wd::Mylogger::getInstance()->info(addprefix(msg))
#define LogDebug(msg) wd::Mylogger::getInstance()->debug(addprefix(msg)) 

#endif
