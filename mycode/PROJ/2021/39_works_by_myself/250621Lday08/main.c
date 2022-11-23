#include <func.h>

int main(int argc,char*argv[]){
    int i=0;
    for(i=0;i<2;i++)
    {
        fork();
        printf("-\n");

    }
    return 0;

}

