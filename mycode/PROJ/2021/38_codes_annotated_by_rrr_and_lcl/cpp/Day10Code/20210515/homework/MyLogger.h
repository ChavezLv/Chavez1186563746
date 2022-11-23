#ifndef __MY_LPGGER_H__
#define __MY_LPGGER_H__
#include <log4cpp/Category.hh>

using namespace log4cpp;//引入log4cpp空间的所有实体

class Mylogger//【mylogger类】
{
public://公开接口
    static Mylogger *getInstance();//相当于：单例模式，全局只允许存在唯一实例---【日志系统mylogger】
    //↑这个函数必是静态成员函数，调用不依托于对象
    static void destroy();//清理函数

    void warn(const char * msg);//记录warn+提供相关信息，一下同理
    void error(const char * msg);
    void debug(const char * msg);
    void info(const char * msg);

private:
    Mylogger();//构造+析构隐藏在private，目的就是仅仅允许创建堆变量
    ~Mylogger();
private:
    static Mylogger *_pInstance;//私有+静态成员
    Category &_mycat;//catedory格式类

};

#define prefix(msg) (string(__FILE__) + string(" ") + string(__FUNCTION__) \
        + string(" ") + string(std::to_string(__LINE__)) + string(" ") \
        + msg).c_str()
//这个宏之所以要用参数宏，是为了保证【__FILE__】这些宏传进去的东西是【位置正确的】//其实这个做法的话在外面取好这几个值作为参数传入也一样
#define LogError(msg)  Mylogger::getInstance()->error(prefix(msg))
#define LogInfo(msg)  Mylogger::getInstance()->info(prefix(msg))
#define LogWarn(msg)  Mylogger::getInstance()->warn(prefix(msg))//调用也写成参数宏，宏的原因和改进都同上
#define LogDebug(msg)  Mylogger::getInstance()->debug(prefix(msg))

#endif
