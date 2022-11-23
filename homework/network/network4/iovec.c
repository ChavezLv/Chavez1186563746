//分散写
    #include<func.h>
int main(){
    char buff1[64]="hello";
    char buff2[20]="world";
    int fd;
    fd =open("file",O_RDWR|O_CREAT);
    struct iovec vecs[2];
    vecs[0].iov_base=buff1;
    vecs[0].iov_len=strlen(buff1);
    vecs[1].iov_base=buff2;
    vecs[1].iov_len=strlen(buff2);
    int ret =writev(fd,vecs,2);
    printf("ret:%d\n",ret);
    close(fd);

}
