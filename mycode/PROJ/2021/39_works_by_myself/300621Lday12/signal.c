#include <func.h>
void sigFunc(int sig){
    sig = 2;
    /* int sign = 3; */
    printf("sig is %d.\n",sig);
}
int main(int argc,char*argv[]){
    /* if(SIG_ERR==signal(2,sigFunc)){ */
    if(SIG_ERR==signal(2,NULL)){
        ERROR_CHECK(SIG_ERR,SIG_ERR,"signal");
    }else{
        printf("signal!\n");
        sigset_t set;
        sigemptyset(&set);
        sigaddset(&set,2);
        sigprocmask(SIG_SETMASK,&set,NULL);
        
        sigset_t pend;
        sleep(2);
        sigpending(&pend);
        if(sigismember(&pend,2)){
            printf("pend2\n");
        }
        while(1);
    }
    return 0;
}

