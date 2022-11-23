#include <func.h>
int main(int argc, char *argv[])
{
    ARGS_CHECK(argc,2);
    int fd = open(argv[1],O_RDWR,0666);
    ERROR_CHECK(fd,-1,"open");
    printf("fd = %d\n", fd);
    int val = 10000000;
    int ret = write(fd,&val,sizeof(val));
    ERROR_CHECK(ret,-1,"write");
    close(fd);
    return 0;
}

