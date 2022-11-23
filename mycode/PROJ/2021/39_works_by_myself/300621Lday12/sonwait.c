#include <func.h>

int main(int argc,char*argv[]){
    if(fork()){
        printf("i am father.\n");
    }else{
        int ret = wait(NULL);
        printf("%d\n",ret);
    }
    return 0;
}

