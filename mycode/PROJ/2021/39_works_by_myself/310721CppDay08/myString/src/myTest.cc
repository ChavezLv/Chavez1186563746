#include "../include/myString.hh"

void test();
int main()
{
    test();
    /* char *p = nullptr; */
    /* strlen(p);//不会core dump */
    /* cout << strlen(p) << endl; */
    /* string s1 = "ss"; */
    /* cout << s1[10] << endl; */
    /* s1 = nullptr; */
    /* cout << s1 << endl; */

    /* string s1 = "hello"; */
    /* cout << s1.size() << endl; */
    return 0;
}

void test(){

    /* String s1(nullptr); */
    /* cout << s1 << endl; */
    /* String s2 = "hhl"; */
    /* cout << s2 << endl; */
    /* s2 = s1; */
    /* cout << s2 << endl; */
    /* String s2("helloworld"); */
    /* cin >> s1; */
    /* cout << s1 << endl; */
    /* string s3; */
    /* cout << s3.size() << endl; */
    /* cout << s2 << endl; */
    const String s1 = "aello";
    const String s2 = "hellO";
    cout << ("dog" + s1 + s2 + "helo") << endl;
    /* s1 = "world"; */
    /* cout << s1 << endl; */
    /* s1 += "1988"; */
    /* s1 += String("1988"); */
    /* s1 += nullptr; */
    /* cout << s1 << endl; */
}
