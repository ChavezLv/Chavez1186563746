#include"head.h"

int main(int argc,char*argv[])
{
    char buf[96] = {0};
    while(1){
        memset(buf, 0, sizeof(buf));
        scanf("%s", buf);
        printf("buf is %s .\n", buf);
        sleep(1);
    }
    return 0;
}

