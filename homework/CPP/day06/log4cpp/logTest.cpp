#include <iostream>

#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/Priority.hh>
#include <log4cpp/Category.hh>
 
using std::cout;
using std::endl;
using namespace log4cpp;
void test0() 
{
	//1.设置日志布局
	PatternLayout * ptn1 = new PatternLayout();
	ptn1->setConversionPattern("%d %c [%p] %m%n");
	//2.创建日志目的地的对象
	OstreamAppender * pos = new OstreamAppender("console", &cout);
	pos->setLayout(ptn1);

	FileAppender * fileApp = new FileAppender("fileApp", "wd.log");
	fileApp->setLayout(ptn1);

	//3.创建日志记录器(模块)
	Category & salesDepart = Category::getRoot().getInstance("salesDepart");
	//3.1 设置模块本身的优先级
	salesDepart.setPriority(Priority::ERROR);
	//3.2 设置日志目的地
	salesDepart.addAppender(pos);
	salesDepart.addAppender(fileApp);

	//每一条日志都有一个优先级, 用Category的成员函数名表示
	//4. 记录日志
	salesDepart.emerg("this is an emerg msg");
	salesDepart.fatal("this is a fatal msg");
	salesDepart.crit("this is a crital msg");
	salesDepart.alert("this is an alert msg");
	salesDepart.error("this is an error msg");
	salesDepart.warn("this is a warn msg");
	salesDepart.info("this is an info msg");
	salesDepart.notice("this is a notice msg");
	salesDepart.debug("this is a debug msg");

	//5.日志系统要退出,Category对象内部会管理好资源的
	Category::shutdown();
} 
 
int main(void)
{
	test0();
	return 0;
}
