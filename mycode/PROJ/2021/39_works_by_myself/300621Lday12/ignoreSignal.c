#include <func.h>
void sigFunc(int sig){
    printf("sig is %d.\n",sig);
}
int main(int argc,char*argv[]){
    /* if(SIG_ERR==signal(2,SIG_IGN)){ */
    if(SIG_ERR==signal(2,SIG_DFL)){
        ERROR_CHECK(SIG_ERR,SIG_ERR,"signal");
    }else{
        printf("signal!\n");
        while(1);
    }
    return 0;
}

