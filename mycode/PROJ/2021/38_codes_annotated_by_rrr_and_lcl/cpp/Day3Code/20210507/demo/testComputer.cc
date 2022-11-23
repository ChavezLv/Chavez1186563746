#include "Computer.h"
#include <iostream>

using std::cout;
using std::endl;

int main(int argc, char **argv)
{
    //对象的创建
    Computer com;
    com.setBrand("huawei");
    com.setPrice(6000);
    com.print();

    return 0;
}
