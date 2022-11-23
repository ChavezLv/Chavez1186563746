#include <func.h>
int main(int argc, char *argv[])
{
    ARGS_CHECK(argc,2);
    int fd = open(argv[1],O_RDWR); //打开文件
    ERROR_CHECK(fd,-1,"open");
    int ret = ftruncate(fd,5); // 固定文件大小
    ERROR_CHECK(ret,-1,"ftruncate");
    char *p = (char *)mmap(NULL,5,PROT_WRITE|PROT_READ,MAP_SHARED,fd,0);
    ERROR_CHECK(p, MAP_FAILED, "mmap");//建立文件和内存的映射，映射区在堆空间
    p[5] = '\0';//p[0]~p[4]映射区，所以这句不会影响文件
    puts(p);//读文件内容
    p[0] = 'H';//修改映射区，相对于修改文件
    munmap(p,5);//解除映射
    close(fd);
    return 0;
}

