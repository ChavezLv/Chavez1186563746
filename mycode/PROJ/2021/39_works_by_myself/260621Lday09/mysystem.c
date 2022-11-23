#include <func.h>
int mysystem(char *cmdstring){
    if(cmdstring == NULL){
        return 1;
    }
    int pid = 0;
    if((pid = fork())==0){
        execl(cmdstring,cmdstring,NULL);
    }else if(pid<0){
        return -1;
    }else{
        wait(NULL);
        return 0;
    }
}
int main(int argc,char*argv[]){
    ARGS_CHECK(argc,3);
    int ret = mysystem(argv[1]);
    int ret_sys = system(argv[2]);
    printf("mysystem ret:%d,system ret:%d\n",ret,ret_sys);
    return 0;
}

