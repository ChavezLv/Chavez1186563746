#include <func.h>
int main(int argc, char *argv[])
{
    ARGS_CHECK(argc,2);
    int fdw = open(argv[1], O_WRONLY);
    int cnt = 0;
    while(1){
        printf("cnt = %d\n", cnt);
        ++cnt;
        write(fdw,"helloworld",10);
    }
    return 0;
}

