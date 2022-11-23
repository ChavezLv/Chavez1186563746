#include <func.h>

int main(int argc,char*argv[]){
    int fd[2] = {0};
    int ret = pipe(fd);
    ERROR_CHECK(ret,-1,"pipe");
    if(fork()){
        write(fd[1],"hello",5);
        wait(NULL);
    }else{
        char buf[10] = {0};
        ret = read(fd[0],buf,sizeof(buf));
        ERROR_CHECK(ret,-1,"read");
        printf("%s\n",buf);
    }
    return 0;
}

