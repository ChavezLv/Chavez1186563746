
#include<func.h>
int main(int argc,char* argv[]){
    ARGS_CHECK(argc,3);
    pid_t pid;
    int signo;
    sscanf(argv[2],"%d",&signo);
    sscanf(argv[1],"%d",&pid);
    kill(pid,signo);
    return 0 ;
}
