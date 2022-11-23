#include"Rss.h"
void test0(){
    RssReader rssr;
    const char* filename="coolshell.xml";
    rssr.parseRss(filename);
    rssr.dump("myxml.txt");
}
int main(){
    test0();
    return 0;
}
