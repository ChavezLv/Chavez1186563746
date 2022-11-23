#include <func.h>

int main(int argc,char*argv[]){
    ARGS_CHECK(argc,3);
    //execl("./num","./num",argv[1],argv[2],NULL);
    system("./num");
    return 0;
}

