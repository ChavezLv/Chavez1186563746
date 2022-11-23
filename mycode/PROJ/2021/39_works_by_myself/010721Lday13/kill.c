#include <func.h>

int main(int argc,char*argv[]){
    ARGS_CHECK(argc,3);
    // 9 pid   只能接受9 而不是 -9
    int ret = kill(atoi(argv[2]),(int)atoi(argv[1]));
    ERROR_CHECK(ret,-1,"kill");
    return 0;
}

