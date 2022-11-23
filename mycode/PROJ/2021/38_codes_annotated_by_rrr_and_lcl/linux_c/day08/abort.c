#include <func.h>
int main()
{
    pid_t pid = fork();
    if(pid == 0){
        printf("pid = %d, ppid = %d\n",getpid(), getppid());
        while(1);
        //abort();
    }
    else{
        printf("I am parent\n");
        int status;
        //wait(&status);
        while(1){
            sleep(1);
            int ret = waitpid(pid,&status,WNOHANG);
            
            if(ret == 0){
                printf("no child state was changed!\n");
            }
            else{
                if(WIFEXITED(status)){
                    printf("exit status = %d\n",WEXITSTATUS(status));
                }
                else if(WIFSIGNALED(status)){
                    printf("crash , sig num = %d\n",WTERMSIG(status));
                }
                break;
            }
        }
    }
}

