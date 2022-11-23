#include <func.h>
int main()
{
    //execl("./add","./add", "1", "2",NULL);// ./add 1 2
    char * const argv[] = {"./add","1","2",NULL};
    execv("./add",argv);
    printf("can you see me?\n");
    return 0;
}

