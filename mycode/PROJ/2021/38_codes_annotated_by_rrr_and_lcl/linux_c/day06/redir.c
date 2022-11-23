#include <func.h>
int main(int argc, char *argv[])
{
    ARGS_CHECK(argc,2);
    int fd = open(argv[1],O_RDWR);
    printf("\n");//重定向之前先打印一个换行
    close(STDOUT_FILENO);//STDOUT_FILENO就是1，表示关闭一号文件描述符
    int newfd = dup(fd);//newfd是1
    printf("newfd = %d\n", newfd);//printf --> stdout --> stdout->fileno==1
    printf("You can't see me!\n");
    close(fd);
    close(newfd);
    return 0;
}

