#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
class Solution {
public:

};
void test(){
    int *pInt = (int*)calloc(1, sizeof(int));
    *pInt  = 10;
    cout << "*pInt = " << *pInt << endl;
    cout << "pInt" << pInt << endl;
    free(pInt);
    cout << "*pInt = " << *pInt << endl;
    cout << "pInt" << pInt << endl;
    pInt = nullptr;
    cout << "pInt" << pInt << endl;
}
void test2()
{
    int* pInt = new int(10);//[10]();//(int*)malloc(sizeof(int));//new int(10);//1、申请堆空间，并且进行初始化，还可以赋值
    cout << "*pInt = " << *pInt << endl;
    cout << "pInt" << pInt << endl;
    cout << "&pInt" << &pInt << endl;
    delete pInt;//2、释放空间
    cout << "*pInt = " << *pInt << endl;
    cout << "before = null: pInt" << pInt << endl;
    cout << "&pInt" << &pInt << endl;
    pInt = nullptr;//NULL
    cout << "pInt: " << pInt << endl;
    cout << "&pInt: " << &pInt << endl;
    int* pArray = new int[10]();
    pArray[0] = 10;

    delete[] pArray;//不能省略，里面不能添加数字

}
int main()
{
    test();
    /* test2(); */
    return 0;
}

