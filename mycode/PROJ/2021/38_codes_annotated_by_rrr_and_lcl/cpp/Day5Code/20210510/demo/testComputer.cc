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


void test3()
{
    Computer com1("huawei", 6000);
    Computer com2("xiaomi", 8000);
    cout << "com1 = ";
    com1.print();
    cout << "com2 = ";
    com2.print();

    cout << endl << "执行com2 = com1" << endl;
    com2 = com1;
    cout << "com1 = ";
    com1.print();
    cout << "com2 = ";
    com2.print();

    com1 = com1;

}

void test4()
{
    cout << "sizeof(Computer) = " << sizeof(Computer) << endl;

    cout << "一台电脑都没有购买 : " << endl;
    Computer::printTotalPrice();

    cout << endl;
    Computer com1("huawei", 6000);
    cout << "com1 = ";
    com1.print();
    cout << "购买第一台电脑总价 : " << endl;
    com1.printTotalPrice();
    Computer::printTotalPrice();

    cout << endl;
    Computer com2("xiaomi", 8000);
    cout << "com2 = ";
    com2.print();
    cout << "购买第二台电脑总价 : " << endl;
    com2.printTotalPrice(); 
    com1.printTotalPrice(); 
    Computer::printTotalPrice();//可以使用类名加作用域限定符进行调用
    /* Computer::print();//error */
}

void test5()
{
    //非const对象既可以访问非const版本成员函数也可以访问
    //const版本的成员函数，但是默认情况下访问非const版本
    //的成员函数
    Computer com1("huawei", 6000);
    cout << "com1 = ";
    com1.print();

    cout << endl;
    //const int number = 10;
    //const对象只能访问const版本的成员函数，不能访问非const版本的成员函数
    const Computer com2("xiaomi", 8000);
    cout << "com2 = ";
    com2.print();

}
int main(int argc, char **argv)
{
    test5();
    return 0;
}
