//分散读
    #include<func.h>
int main(){
    char buff1[5]={0};
    char buff2[10]={0};
    int fd;
    fd =open("file",O_RDWR);
    struct iovec vecs[2];
    vecs[0].iov_base=buff1;
    vecs[0].iov_len=5;//必须预留一个空位,因为读取到"\0"才结束
    vecs[1].iov_base=buff2;
    vecs[1].iov_len=sizeof(buff2);
    int ret =readv(fd,vecs,2);
    printf("ret:%d\n",ret);//10字符+1个"\0"
    printf("buf1:%s\nbuf2:%s\n",buff1,buff2);
    //close(fd);
    return 0;
}
