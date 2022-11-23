/*================================================================
*   Copyright (C) 2021 anonymous All rights reserved.
*   
*   @file       ：01testConstMemberFunction.c
*   @author     ：anonymous
*   @date       ：2021年09月09日 10:31:40
*   @description：
*
================================================================*/


    const int BUFSZIE = 100;
#include <iostream>
using namespace std;
class Text{
    private:
        /* int k;  //mutable int k; */
        mutable int k;
        const static int a = 10;
        const static char b = 10;
    public:
        Text(int kk){
            k=kk;
        }
        void printconst() const{
            /* k=1000;            //不能修改类的成员变量 */
            k = 1000;
            cout<<k<<endl;
        }
        void print(){
            cout<<k<<endl;
        }

        void constFunc(const int &i){
            /* ++i; */
        }
};

void test(){

    int *pInt = new int (10);
    int *pInt1 = new int [10];
    int *pInt2 = new int [10] ();
    /* int *pInt3 = new int [10] (10);//error */

    char *pChar01 = new char('c');
    char * pChar = new char[100];
    char * pChar1 = new char[100]();
}
int main(){
    Text e(100);
    e.print();
    const Text t=e;
    //t.print();    //不能调用普通成员函数
    t.printconst();
    return 0;
}
