#include<func.h>

int main(int argc,char*argv[]){
    ARGS_CHECK(argc,3);
    mkdir(argv[1],0777);
    rmdir(argv[2]);
    return 0;
}
