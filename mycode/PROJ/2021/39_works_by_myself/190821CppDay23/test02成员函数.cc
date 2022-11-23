#include <iostream>
using std::cin;
using std::cout;
using std::endl;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <deque>
using std::deque;

#include <list>
using std::list;

#include <map>
using std::map;
#include <functional>
using std::function;
using std::bind;

class Solution {
public:
    int value;
    void add(){
        cout << "add: " << value << endl;
    }
    static void multi(){
        cout << "multi: " << endl;
    }

};
typedef void (*pFunc1)();
typedef void (Solution::*pFunc)();
typedef void (*pFunc2)();
void func(Solution * pSol, void (Solution::*fun)()){
    (pSol->*fun)();
}
void add(){
    cout << "add()" << endl;
}
void test01(){
    Solution sol{0};
    Solution sol1{1};
    /* auto f = bind(&Solution::add, &sol); //ok*/
    /* function<void()> f = bind(&Solution::add, &sol); //ok */
    /* pFunc f = bind(&Solution::add, &sol); */
    pFunc f = &Solution::add;
    (sol1.*(&Solution::add))();//sol1.add();
    (sol1.*f)();

    Solution *p = &sol;
    (p->*f)();

    /* pFunc1 f1 = add; */
    /* f1(); */

    //静态成员函数不需要取地址
    /* Solution::multi; //ok*/
    /* pFunc1 staticMemberFunc = Solution::multi; */
}

class Test02{
public:
    virtual
    void add(){

    }
    static void func(){

    }
    static int value1;
    int value;//函数不占大小
    char c;

};
//测试空类以及类的大小
void test02(){
    Test02 ts;
    cout << sizeof(Test02) << endl;
    cout << sizeof(ts) << endl;
}
int main(int argc, char* argv[])
{

    /* test01(); */
    test02();
    return 0;
}

