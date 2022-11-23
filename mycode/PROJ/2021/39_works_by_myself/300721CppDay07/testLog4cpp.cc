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

using namespace log4cpp; //一次性将log4cpp的实体全部引出
/* using namespace std; */
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

class Solution {
public:
    static void test();

};

void Solution::test(){
    //layout: a plan or design of something that is laid out
    SimpleLayout *ps1 = new SimpleLayout();

    //appender of log
    OstreamAppender *pos = new OstreamAppender("OsApp123", &cout);
    pos->setLayout(ps1);

    //category of log 
    Category &root = Category::getRoot();
    root.addAppender(pos);
    root.setPriority(Priority::ERROR);

    root.fatal("1");
    root.alert("2");
    root.crit("3");
    root.error("4");
    root.warn("5");

    Category::shutdown();
}
int main()
{
    Solution::test();
    return 0;
}

