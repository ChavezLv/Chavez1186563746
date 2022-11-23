/*================================================================
*   Copyright (C) 2021 anonymous All rights reserved.
*   
*   @file       ：01testSE.cpp
*   @author     ：anonymous
*   @date       ：2021年09月02日 15:52:43
*   @description：
*
================================================================*/

#include "SearchEngineServer.hpp" 

using namespace wd;
namespace wd{

}//end of namespace wd

void test01(){
    SearchEngineServer searchEngineServer(8888);
    searchEngineServer.start();

}

int  main(){
    test01();
    return 0;
}

