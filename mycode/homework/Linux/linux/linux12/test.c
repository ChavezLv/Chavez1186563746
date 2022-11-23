#include<func.h>
int main(int argc,char* argv[]){
    printf("hello world\n");
    ARGS_CHECK(argc,1);
    ERROR_CHECK(2,2,"fdjsf");
    return 0;

}
