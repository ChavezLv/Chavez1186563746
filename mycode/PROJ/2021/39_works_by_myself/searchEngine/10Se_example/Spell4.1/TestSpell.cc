#include "SpellcorrectServer.hpp"

using namespace::wd;
int main(int argc,char** argv)
{
    if(argc!=2){
        perror("argument error!");
    }  
    SpellcorrectServer server(argv[1]);
    server.start();
   
    /* server.stop(); */
    return 0;
}

