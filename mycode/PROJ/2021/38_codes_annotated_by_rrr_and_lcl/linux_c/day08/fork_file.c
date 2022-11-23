#include <func.h>
int main()
{
    printf("hello");
    if(fork() == 0){
        printf("world\n");
    }
    else{
        printf("world\n");
    }
    return 0;
}

