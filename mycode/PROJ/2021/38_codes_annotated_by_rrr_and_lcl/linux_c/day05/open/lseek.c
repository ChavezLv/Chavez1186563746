#include <func.h>
int main(int argc, char *argv[])
{
    ARGS_CHECK(argc,2);
    int fd = open(argv[1],O_RDWR);
    ERROR_CHECK(fd,-1,"open");
    lseek(fd,40960,SEEK_SET);
    write(fd,"1",1);
    close(fd);
    return 0;
}

