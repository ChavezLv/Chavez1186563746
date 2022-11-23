#include <func.h>

int main(int argc,char*argv[]){
    int pid = 0;
    if((pid=fork())==0){
        puts("child");
    }else{
        
    }
    return 0;
}

