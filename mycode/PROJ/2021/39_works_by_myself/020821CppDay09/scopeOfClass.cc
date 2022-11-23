#include <iostream>

using std::cout;
using std::endl;

int x = 1;

namespace wd
{
int x = 10;

class Test
{
public:
    Test(int value)
        : x(value)
    {


    }

    void print(int x) const
    {
        cout << "形参x = " << x << endl;
        cout << "数据成员x = " << this->x << endl;
        cout << "数据成员x = " << Test::x << endl;
        cout << "命名空间中的x = " << wd::x << endl;
        cout << "全局的x = " << ::x << endl;

    }
private:
    int x;

};

}//end of namespace wd
int main(int argc, char **argv)
{
    int value = 300;
    wd::Test tst(400);
    tst.print(value);

    return 0;

}


