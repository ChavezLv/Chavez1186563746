#include <func.h>
int main(int argc, char *argv[])
{
    ARGS_CHECK(argc,2);
    int fd1 = open(argv[1],O_RDWR);
    ERROR_CHECK(fd1,-1,"open");
    printf("fd1 = %d\n",fd1);
    int fd2 = dup(fd1);
    ERROR_CHECK(fd2,-1,"dup");
    printf("fd2 = %d\n",fd2);
    close(fd1);
    write(fd2,"world",5);
    close(fd2);
    return 0;
}

