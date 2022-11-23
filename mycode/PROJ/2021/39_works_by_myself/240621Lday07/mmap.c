#include<func.h>

int main(int argc,char*argv[]){
    ARGS_CHECK(argc,2);
    int fd = open(argv[1],O_RDWR);
    ERROR_CHECK(fd,-1,"open");

    char buf[10] = {0};
    read(fd,buf,sizeof(buf));
    puts(buf);
    ftruncate(fd,5);
    char *p = (char*)mmap(NULL,5,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    strcpy(p,"world");
    puts(p);
    return 0;
}
