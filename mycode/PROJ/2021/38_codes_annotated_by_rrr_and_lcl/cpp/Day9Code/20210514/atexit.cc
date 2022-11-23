#include <stdlib.h>
#include <iostream>

using std::cout;
using std::endl;


void func()
{
    cout << "void func()" << endl;
}

void test()
{
    atexit(func);
    atexit(func);
    atexit(func);
    atexit(func);
    atexit(func);
}
int main(int argc, char **argv)
{
    cout << "begin ..." << endl;
    test();
    cout << "finish..." << endl;
    return 0;
}

