#include <iostream>

using std::cout;
using std::endl;

int main(int argc, char **argv)
{
    int *pInt = new int(10);

    delete pInt;
    pInt = nullptr;
    return 0;
}

