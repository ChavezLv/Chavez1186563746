#include <iostream>
#include <string>
#include <limits>

using std::cout;
using std::endl;
using std::cin;
using std::string;

void printStreamStatus()
{
    cout << "cin.bad = " << cin.bad() << endl
         << "cin.fail = " << cin.fail() << endl
         << "cin.eof = " << cin.eof() << endl
         << "cin.good = " << cin.good() << endl;
}
void test()
{
    int number = 10;
    printStreamStatus();
    cin >> number;
    printStreamStatus();
    cin.clear();//重置流的状态
    /* cin.ignore(1024, '\n');//清空缓冲区 */
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');//清空缓冲区
    printStreamStatus();

    cout << "number = " << number << endl;

    string s1;
    cin >> s1;
    cout << "s1 = " << s1 << endl;
}

void test2()
{
    int number = 10;
    //逗号表达式
    while(cin >> number, !cin.eof())
    {
        if(cin.bad())
        {
            std::cerr << "The stream is bad" << endl;
            return;
        }
        else if(cin.fail())
        {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');//清空缓冲区
            cout << "请输出int类型的数据" << endl;
        }
        else
        {
            cout << "number = " << number << endl;
        }
    }
}
int main(int argc, char **argv)
{
    test2();
    return 0;
}

