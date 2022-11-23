#include <func.h>
int main(int argc, char *argv[])
{
    ARGS_CHECK(argc,2);
    int fd1 = open(argv[1],O_RDWR);
    printf("fd1 = %d\n", fd1);

    dup2(STDOUT_FILENO,5);
    dup2(fd1,STDOUT_FILENO);
    printf("You can't see me!\n");

    dup2(5,STDOUT_FILENO);
    printf("You can see me!\n");

    close(fd1);
    close(5);
    return 0;
}

