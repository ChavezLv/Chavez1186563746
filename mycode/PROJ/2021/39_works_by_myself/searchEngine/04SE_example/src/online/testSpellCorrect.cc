#include "../../include/SpellCorrectServer.hpp"
#include "../../include/MyTask.hpp"

#include <iostream>

using std::cout;
using std::endl;
using namespace spellCorrect;

void test0()
{
    SpellCorrectServer server("../conf/");
    server.start();
}
#if 1
int main()
{
    test0();
    return 0;
}
#endif

