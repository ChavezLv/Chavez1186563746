#include <unistd.h>
#include <iostream>

using std::cout;
using std::endl;
using std::ends;
using std::flush;

void test()
{
    for(size_t idx = 0; idx != 1024; ++idx)
    {
        cout << 'a';
    }
    /* cout << "123" << endl;//endl是一个函数，具有刷新缓冲区的功能，同时可以换行 */
    /* cout.operator<<(endl); */
    /* cout << "123" << flush;//flush是一个函数，具有刷新缓冲区的功能，没有换行功能 */
    cout << "123" << ends;//ends是一个函数，没有刷新缓冲区的功能，不能换行

    sleep(5);
}
int main(int argc, char **argv)
{
    test();
    return 0;
}

