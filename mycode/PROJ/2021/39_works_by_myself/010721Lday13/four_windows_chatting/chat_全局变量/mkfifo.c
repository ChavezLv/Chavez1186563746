#include <func.h>

int main(int argc,char*argv[]){
    int ret;
    ARGS_CHECK(argc,3);
    ret = mkfifo(argv[1],IPC_CREAT|0600);
    ERROR_CHECK(ret,-1,"mkfifo");
    ret = mkfifo(argv[2],IPC_CREAT|0600);
    ERROR_CHECK(ret,-1,"mkfifo");
    return 0;
}

