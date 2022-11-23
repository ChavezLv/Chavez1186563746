#define __LOG4CPP__
#include "../../libs/Mylog.hh"
#include "../../include/tasklib/SpellCorrectServer.hh"

#include <iostream>

using std::cout;
using std::cerr;
using std::endl;

int main(int args, char * argv[])
{
    if(args != 2) {
        cerr << "need confPath" << endl;
    }
    Mylog::getInstance().setPriority(INFO);
    wd::SpellCorrectServer server(argv[1]);
    server.start();
    return 0;
}

