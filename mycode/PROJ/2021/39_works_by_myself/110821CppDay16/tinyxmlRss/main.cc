#include "rss2pagelib.hh"
#include "tinyxml2.h"

void test(char *argv[]){
    /* RssReader().parseRss("coolshell.xml"); */
    /* RssReader().dump("pagelib.txt"); */
    RssReader rss;
    rss.parseRss(argv[1]);
    rss.dump("pagelib.txt");

}
int main(int argcs, char* argv[]){
    if(argcs != 2){
        std::cout << "error argcs" << std::endl;
    }
    test(argv);
    return 0;
}



