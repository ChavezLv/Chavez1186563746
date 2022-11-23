#define _LOG4CPP_

#include "../include/MyLog.hpp"
#include "../include/Myconf.hpp"
#include "../include/SpellCorrectServer.hpp"
#include "../include/Mydict.hpp"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

int main(int argc,char *argv[])
{
    wd::Myconf conf("../conf/server.conf");
    
    wd::Mydict *pInstance = wd::Mydict::createInstance();    
    pInstance->init(conf.getConfigMap()["enDictPath"],conf.getConfigMap()["enIndexTablePath"],
                    conf.getConfigMap()["cnDictPath"],conf.getConfigMap()["cnIndexTablePath"]);
    wd::SpellCorrectServer scServer(conf);
    
    scServer.start();

    return 0;
}

