#include <func.h>
int main(int argc, char *argv[])
{
    ARGS_CHECK(argc,2);
    int fd  = open(argv[1],O_RDWR);
    ERROR_CHECK(fd,-1,"open");
    if(fork() == 0){
        write(fd,"hello",5);
    }
    else{
        write(fd,"world",5);
    }
    return 0;
}

