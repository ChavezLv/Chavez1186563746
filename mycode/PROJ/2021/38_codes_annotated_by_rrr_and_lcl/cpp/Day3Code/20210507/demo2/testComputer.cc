#include "Computer.h"
#include <iostream>

using std::cout;
using std::endl;

/* Computer gCom("xiaomi", 8000);//全局对象，位于全局区 */

void test()
{
    cout << "start main... " << endl;
    //对象的创建
    Computer com("huawei", 6000);//栈对象
    /* com.setBrand("huawei"); */
    /* com.setPrice(6000); */

    /* com.~Computer();//不要显示调用析构函数 */

    /* com.print(); */

    /* int *pInt = new int(10); */

    Computer *pc = new Computer("mac", 20000);//堆对象一定要进行delete操作
    pc->print();

    delete pc;

    cout << "finish main..." << endl;

}

void test2()
{
    Computer com1("huawei", 6000);
    cout << "com1 = ";
    com1.print();


    cout << endl;
    Computer com2 = com1;
    cout << "com2 = ";
    com2.print();

    //浅拷贝的时候，用com1.setBrand("xiaomi")
    //可以看到com2的brand也变成了"xiaomi"
}

int main(int argc, char **argv)
{
    test2();
    return 0;
}
