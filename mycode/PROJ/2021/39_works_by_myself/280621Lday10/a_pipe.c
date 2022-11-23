#include <func.h>
int fun1(int fd[]){
    int ret = 0;
    if(fork()){
        write(fd[1],"hello",5);
        char buf[10] = {0};
        /* printf("dad\n"); */
        ret = read(fd[0],buf,sizeof(buf));
        ERROR_CHECK(ret,-1,"read");
        printf("dat ret read :%d\n",ret);
        printf("dad:%s\n",buf);
        printf("dad\n");
        ret = read(fd[0],buf,sizeof(buf));
        wait(NULL);
    }else{
        write(fd[1],"hello",5);
        char buf[10] = {0};
        ret = read(fd[0],buf,sizeof(buf));
        ERROR_CHECK(ret,-1,"read");
        printf("son read :%d\n",ret);
        printf("son:%s\n",buf);
        ret = write(fd[1],"hello",5);
        printf("son write:%d  %d\n",ret,ret);
        ERROR_CHECK(ret,-1,"write");
    }
    return 0;
}
int fun2(int fd[]){
    char buf[10] = {0};
    if(fork()){
        write(fd[1],"hello",5);
    }else{
        write(fd[1],"hello",5);
        read(fd[0],buf,3);
        puts(buf);
        memset(buf,0,sizeof(buf));
        read(fd[0],buf,3);
        puts(buf);
    }
    return 0;
}
int fun3(int fd[]){
    char buf[10] = {0};
    if(fork()){
        write(fd[1],"hello",5);
    }else{
        write(fd[1],"hello",5);
        read(fd[0],buf,10);
        puts(buf);
    }
    return 0;
}
int fun4(int fd[]){
    /*
     *flush
     p->d:worlds:h
     pp->ellos:hell

     flush
     pp->ello
     p->d:worlds:h
     * */
    char buf[10] = {0};
    if(fork()){ 
        write(fd[1],"d:world",7);
        printf("flush\n");//没有这个会单进程内自己write,自己read
        read(fd[0],buf,10);
        printf("pp->%s\n",buf);
        wait(NULL);
    }else{
        write(fd[1],"s:hello",7);
        read(fd[0],buf,10);
        printf("p->%s\n",buf);
        write(fd[1],"s:hello",7);
    }
    return 0;
}
int fun5(int fd[]){
    /*
     *pp->d:world
     * */
    char buf[10]={0};
    if(fork()){
        write(fd[1],"d:world",7);
        printf("flush\n");//没有这个会单进程内自己write,自己read
        read(fd[0],buf,10);
        printf("pp->%s\n",buf);
        wait(NULL);
    }else{
        int ret = read(fd[0],buf,10);
        printf("%d",ret);
        printf("p->%s\n",buf);
        write(fd[1],"s:hello",7);
    }
    return 0;
}
int main(int argc,char*argv[]){
    int fd[2] = {0};
    int ret = pipe(fd);
    ERROR_CHECK(ret,-1,"pipe");
    fun4(fd);
    return 0;
}
