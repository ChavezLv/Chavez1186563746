#include <func.h>
void sigFunc(int sig){
    printf("sig is %d.\n",sig);
}
int main(int argc,char*argv[]){
    struct sigaction sigactS;
    memset(&sigactS,0,sizeof(sigactS));

    sigactS.sa_handler = sigFunc;
    sigactS.sa_flags = 0;

    int ret = sigaction(2,&sigactS,NULL);
    ERROR_CHECK(ret,-1,"sigaction");

    while(1);
    return 0;
}

