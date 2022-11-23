#include <func.h>
int main()
{
    //双工管道，提前准备好两个方向的
    int fds_A_B[2];
    int fds_B_A[2];
    pipe(fds_A_B);//一定记得是【文件描述符】数组的名字，你拿去给函数填的
    pipe(fds_B_A);
    
    if(fork() == 0){//子是B，主是A
        close(fds1[0]);//关闭子进程的读端
        close(fds2[1]);
        write(fds1[1],"nihaohuai",9);
        char buf[128] = {0};
        read(fds2[0],buf,sizeof(buf));
        printf("child: %s\n", buf);
        exit(0);
    }
    else{
        close(fds1[1]);//父进程关闭写端
        close(fds2[0]);
        char buf[128] = {0};
        read(fds1[0],buf,sizeof(buf));
        printf("parent: %s\n",buf);
        write(fds2[1],"heiheihei",9);
        wait(NULL);
        exit(0);
    }
}

