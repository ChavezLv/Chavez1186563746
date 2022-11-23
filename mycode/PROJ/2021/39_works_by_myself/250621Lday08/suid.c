#include <func.h>
/*往file文件中写入内容hello，然后编译后，增加s权限，这时B用户执行该程序，能够往file文件中写入hello*/
int main(int argc,char*argv[]){
    ARGS_CHECK(argc,2);
    int fd = open(argv[1],O_WRONLY);
    ERROR_CHECK(fd,-1,"open");
    write(fd,"hello",5);
    printf("uid:%d.\n",getuid());
    printf("gid:%d.\n",getgid());
    printf("euid:%d.\n",geteuid());
    printf("egid:%d.\n",getegid());
    return 0;

}

