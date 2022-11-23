#include <func.h>

int main(int argc,char*argv[]){
    char buf[1024] = "aaaa";
    char query[4096] = { 0  };
    sprintf(query, "insert into USER_INFO (userName, salt) values ('%s','%s')", buf, "6k");
    puts(query);
    return 0;
}

