#include <func.h>
int main(int argc, char *argv[])
{
    ARGS_CHECK(argc,2);
    int fd = open(argv[1], O_RDONLY);
    printf("OK!\n");
    char buf[128] = {0};
    read(fd,buf,128);
    puts(buf);
    close(fd);
    return 0;
}

