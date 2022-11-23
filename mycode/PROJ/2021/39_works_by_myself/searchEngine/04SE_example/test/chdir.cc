#include <unistd.h>

#include <iostream>

using std::cout;
using std::endl;

void test1()
{
    cout << ">> test1()" << endl;
    char path[128];
    char* ret = getcwd(path,sizeof(path));
    cout << "ret = " << ret << endl;
    cout << "path = " << path << endl;
    int ret2 = chdir("..");
    cout << "ret2 = " << ret2 << endl;
    cout << "path = " << path << endl;
}

void test2()
{
    cout << ">> test2()" << endl;
    char path[128];
    char* ret = getcwd(path,sizeof(path));
    cout << "ret = " << ret << endl;
    cout << "path = " << path << endl;
    int ret2 = chdir("..");
    cout << "ret2 = " << ret2 << endl;
    char* ret = getcwd(path,sizeof(path));
    cout << "path = " << path << endl;
}

int main()
{
    test1();

    cout << ">> main()" << endl;
    char path[128];
    char* ret = getcwd(path,sizeof(path));
    cout << "ret = " << ret << endl;
    cout << "path = " << path << endl;
    int ret2 = chdir("..");
    cout << "ret2 = " << ret2 << endl;
    cout << "path = " << path << endl;

    test2();
    return 0;
}

