#include <func.h>
int main(int argc, char *argv[])
{
    ARGS_CHECK(argc,2);
    int fd;
    //fd = open(argv[1], O_RDWR);
    fd = open(argv[1],O_WRONLY|O_CREAT|O_EXCL,0666);
    ERROR_CHECK(fd,-1,"open");
    printf("fd = %d\n", fd);
    close(fd);
    return 0;
}

