#include<iostream>
using namespace std;
int globalNum=10;
int & func(){
    int local_num=10;
    cout<<"func()"<<endl;
    globalNum=5;
    cout<<&globalNum<<endl;
    return globalNum;
}
int func1(){
    return 3;
}
int main(){
    //取地址
    typedef int (*f)();
    f F;
        F=func1;
    cout<<F()<<" main "<<endl;
    func()=10;
    return 0;
}
