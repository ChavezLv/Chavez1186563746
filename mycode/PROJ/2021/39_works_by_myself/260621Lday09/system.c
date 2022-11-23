#include <func.h>

int main(int argc,char*argv[]){
    ARGS_CHECK(argc,2);
    int ret = system(argv[1]);
    printf("ret %d\n",ret);
    return 0;
}

