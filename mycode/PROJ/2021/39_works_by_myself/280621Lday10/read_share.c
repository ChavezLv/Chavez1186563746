#include <func.h>

int main(int argc,char*argv[]){

    char *p = (char*)shmat(524293,NULL,0); 
    ERROR_CHECK(p,(void*)-1,"shmat");
    puts(p);
    return 0;
}

