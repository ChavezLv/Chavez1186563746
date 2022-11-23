#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
/* using namespace std; */
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

class Solution {
public:

};
void test(){
    int a = 10;
    int *pInt = new int(10);
    cout << "stack:  " << &a << endl;
    cout << "heap:  " << pInt << endl;

    string s1 = "hello";
    string s2 = "123456789012345";
    string s3 = "1234567890123456";
    cout << "&s1:" << &s1 << endl;
    cout << "&s2:" << &s2 << endl;
    cout << "&s3:" << &s3 << endl;//string 内部的指针_pstr都会是栈的
    printf("&s1:%p\n", s1.c_str());
    printf("&s2:%p\n", s2.c_str());
    printf("&s3:%p\n", s3.c_str());//堆
    
}
int main()
{
    test();
    return 0;
}

