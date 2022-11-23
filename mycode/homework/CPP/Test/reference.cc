#include <iostream>
using std::cout;
using std::endl;

////引用作为函数的返回值, 其生命周期一定要大于函数本身
int g_number=100;
int& func1(){
    //because of the return value is reference that doesnt to do operator of copy
    cout<<"func1() g_number:"<<g_number<<endl;
    return g_number;
}

int func2(){//no problem
    cout<<"func2() g_number:"<<g_number<<endl;
    return g_number;
}

int& func3(){
    int local_number=10;
    return local_number;
}
