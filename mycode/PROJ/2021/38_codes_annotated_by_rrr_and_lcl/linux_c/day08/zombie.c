#include <func.h>
int main()
{
    if(fork() == 0){
        printf("I am child\n");
        return 0;
    }
    else{
        printf("I am parent\n");
        while(1);
    }
}

