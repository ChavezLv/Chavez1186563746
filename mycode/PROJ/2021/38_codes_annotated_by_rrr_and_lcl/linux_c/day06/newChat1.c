#include <func.h>
int main(int argc, char *argv[])
{
    // ./chat1 1.pipe 2.pipe
    ARGS_CHECK(argc,3);
    int fdr = open(argv[1],O_RDONLY); //对于管道来说【读写端都打开】，才叫做【打开】，所以如果开了R模式没人开W模式，就一定会死锁在这里
    int fdw = open(argv[2],O_WRONLY);
    puts("chat1");
    char buf[128] = {0};
    fd_set rdset;
    while(1){
        struct timeval timeout;//超时机制
        timeout.tv_sec = 1;
        timeout.tv_usec = 500000;
        FD_ZERO(&rdset);
        FD_SET(fdr,&rdset);
        FD_SET(STDIN_FILENO,&rdset);
        int nret = select(fdr+1,&rdset,NULL,NULL,&timeout);//SELECT的参数【最大fd+1】，有一种【限制了遍历问询范围】的感觉
        if(nret == 0){
            printf("time out!\n");
        }
        else{
            if(FD_ISSET(fdr,&rdset)){
                printf("msg from pipe\n");
                memset(buf,0,sizeof(buf));
                int ret = read(fdr,buf,sizeof(buf));
                if(ret == 0){//此处是【检测到对端关闭所以自己要处理】（被动）
                    printf("chat is end\n");
                    break;
                    //自己作为对端的退出机制：1-ret=0 + 激活态 查到管道ret=0就需要处理，不然持续的就绪态会让select一直在被唤醒的状态
                    //而这种设定的原因来源于【read的接口设计是希望你有一个方法察觉对面已经关闭的】，并不希望因此就进入睡眠
                }
                puts(buf);
            }
            if(FD_ISSET(STDIN_FILENO,&rdset)){
                printf("msg from stdin\n");
                memset(buf,0,sizeof(buf));
                int ret = read(STDIN_FILENO,buf,sizeof(buf));
                //在stdin里读到eof ，ret值会是0,
                //有一个策略可以探查到对面关了，对端关闭，我就持续激活，
                if(ret == 0){
                    puts("I quit");
                    write(fdw,"nishigehaoren",13);
                    //本机退出机制
                    //1-检测到键盘的eof 2-打印离开的信息 3-发送告别给对端 4-break退出让自己关闭端口
                    //5-其实在这之后才是【对端开始退出机制】
                    break;
                }
                write(fdw,buf,strlen(buf));//正常的发送信息
            }

        }

    }
    close(fdw);//是这个close是【自我关闭】导致对端要退出处理（主动）
    close(fdr);
    return 0;
}

