#include <func.h>

void newSigFunc(int signum,siginfo_t *p,void *u){
    printf("\n");
    printf("%s$",getcwd(NULL,0));
    rewind(stdout);
    /* fflush(stdout); */
}
int main(int argc,char*argv[]){
    struct sigaction sig;
    memset(&sig,0,sizeof(sig));

    sig.sa_sigaction = newSigFunc;
    sig.sa_flags = SA_SIGINFO|SA_RESTART;
    int ret = 1;
    char buf[4096] = {0};
    ret = sigaction(2,&sig,NULL);
    ERROR_CHECK(ret, -1, "sigaction");
    while(1){
        /* printf("\n"); */
        printf("%s$",getcwd(NULL,0));
        fflush(stdout);

        memset(buf,0,sizeof(buf));
        /* fgets(buf,sizeof(buf),stdin); */
        ret = read(STDIN_FILENO,buf,sizeof(buf));
        buf[strlen(buf)-1] = '\0';
        if(strlen(buf)==0){
            continue;
        }
        if(strcmp(buf,"ls") == 0||strcmp(buf,"ls -l") == 0||strcmp(buf,"pwd") == 0||strcmp(buf,"tree") == 0){
            if(!fork()){
                execl("/bin/./bash","/bin/./bash","-c",buf,NULL);
                exit(0);
            }else{
                wait(NULL);
            }
        }else if(strcmp(buf,"exit")==0){
            break;
        }else{
            printf("error!");
            rewind(stdout);
        }
    }
    return 0;
}
