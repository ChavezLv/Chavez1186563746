#include<func.h>


int main(int argc,char* argv[]){
    ARGS_CHECK(argc,3);
    //char buf[] = {0};
    int mode;
    sscanf(argv[2],"%o",&mode);
    chmod(argv[1],mode);
    return 0;

}
