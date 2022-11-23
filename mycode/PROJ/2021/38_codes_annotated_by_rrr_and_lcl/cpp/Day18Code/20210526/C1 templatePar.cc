#include <iostream>

using std::cout;
using std::endl;

//cource1: 
//模板的参数类型
//1、类型参数 T
        //类型参数也是可以设定默认值的: <typename T = int,
//2、非类型参数  整型,bool/char/short/int/long/size_t( 要记忆的 ! )
        //非类型参数时可以设定默认值的: , short kMin = 10>
// 整形是和浮点型不同的, 比如不能传float和double
//

//接下来 course2 了
template <typename T = int, short kMin = 10>
T multiply(T x, T y)
{
    return x * y * kMin;
}
int main(int argc, char **argv)
{
    int ia = 10, ib = 20;
    double da = 3.3, db = 4.4;
    cout << "multiply(ia, ib) = " << multiply(ia, ib) << endl;
    cout << "multiply<int,5>(ia, ib) = " << multiply<int, 5>(ia, ib) << endl;
    cout << multiply<double, 10>(da, db)
    return 0;
}

