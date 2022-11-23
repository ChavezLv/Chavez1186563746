#include <func.h>
int main(int argc, char *argv[])
{
    ARGS_CHECK(argc,2);
    int fd = open(argv[1],O_WRONLY);
    printf("OK\n");
    write(fd,"nihao",5);
    close(fd);
    return 0;
}

