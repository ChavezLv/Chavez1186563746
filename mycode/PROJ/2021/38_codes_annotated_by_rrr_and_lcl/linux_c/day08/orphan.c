#include <func.h>
int main()
{
    if(fork() == 0){
        printf("I am child\n");
        while(1);
    }
    else{
        printf("I am parent\n");
        return 0;
    }
}

