#include<iostream>
using namespace std;
/*测试const成员函数与const对象
 * 对象数组*/
class test{
public:
    test(int i,int j=0)
        :cons(i),integ(j){}
    void func1()const{
        cout<<"func1:"<<cons<<endl;
        cout<<"func1:"<<stat<<endl;
        cout<<"func1:"<<integ<<endl;
    }
    void func2(){
        cout<<"func2"<<endl;
    }
    static void func3(){
        //cout<<"func1:"<<cons<<endl;
        cout<<"func3:"<<stat<<endl;
        //cout<<"func1:"<<integ<<endl;
    }
private:
    static int stat;
    const int cons;
    int integ;
};
int test::stat=9;
int main(){
    test t1(8);
    //t1.func1();

    const test t2(4);
    //t2.func1();
    //t2.func2();
    //t2.func3();

    static test t3(6);
    //t1.func1();
    //t3.func2();
    //t3.func3();
    
    test t4[3]={{1,2},{3,4},{5,6}};
    t4[0].func1();
    return 0;
}
