#include <string>
#include <iostream>

using std::cout;
using std::endl;
using std::string;

void test()
{
    int a = 10;//栈上
    int *pInt = new int(10);//pInt指向的空间是堆
    string s1 = "hello";
    string s2 = "helloworldwuhan";
    string s3 = "welcomehelloworldwuhan";//大于15堆空间
    printf("s1's address : %p\n", s1.c_str());
    printf("s2's address : %p\n", s2.c_str());
    printf("s3's address : %p\n", s3.c_str());
    printf("&a = %p\n", &a);
    printf("&pInt = %p\n", &pInt);
    printf("pInt = %p\n", pInt);

}
int main(int argc, char **argv)
{
    test();
    return 0;
}

