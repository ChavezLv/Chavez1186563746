#include <func.h>
int main()
{
    if(fork() == 0){
        printf("111111111111111\n");
        printf("111111111111111\n");
        execl("./add","./add","3","4",NULL);//execl的要求就是必须NULL结尾
        printf("You can't see me\n");
    }
    else{
        printf("You can see me\n");
    }
    return 0;
}

