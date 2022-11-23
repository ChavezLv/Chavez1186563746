#include <func.h>
/*编写程序，打印当前进程pid，父进程pid，真实用户ID，真实组ID，有效用户ID，有效组ID*/

int main(int argc,char*argv[]){
    printf("pid:%d.\n",getpid());
    printf("ppid:%d.\n",getppid());
    printf("uid:%d.\n",getuid());
    printf("gid:%d.\n",getgid());
    printf("euid:%d.\n",geteuid());
    printf("egid:%d.\n",getegid());
    return 0;
}
