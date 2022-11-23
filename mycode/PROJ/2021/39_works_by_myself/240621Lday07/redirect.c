#include<func.h>

int main(int argc,char *argv[]){

    ARGS_CHECK(argc,3);
    int fd1 = open(argv[1],O_WRONLY);
    ERROR_CHECK(fd1,-1,"open");
    int fd2 = open(argv[2],O_WRONLY);
    ERROR_CHECK(fd2,-1,"open");
    int fd_in = 10;
    int fd_err = 11;
    dup2(STDOUT_FILENO,fd_in);
    dup2(fd1,STDOUT_FILENO);
    dup2(STDERR_FILENO,fd_err);
    dup2(fd2,STDERR_FILENO);

    printf("hello world.\n");
    perror("hello error.\n");
    close(fd1);
    close(fd2);
    close(fd_in);
    close(fd_err);
    return 0;
}

