#include<func.h>

int main(int argc,char *argv[]){

    ARGS_CHECK(argc,2);
    printf("%s\n",getcwd(NULL,0));
    chdir(argv[1]);
    printf("%s\n",getcwd(NULL,0));
    return 0;
}
