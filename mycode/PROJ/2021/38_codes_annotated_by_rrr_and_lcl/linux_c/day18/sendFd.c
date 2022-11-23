#include <head.h>

int main(int argc, char** argv)
{
    int sfd[2];
    pipe(sfd);

    if(fork()){
        close(sfd[0]);
        int fd = open("file", O_RDWR);
        printf("fd = %d\n", fd);
        
        write(sfd[1], &fd, sizeof(int));

        wait(NULL);
    }
    else{
        close(sfd[1]);
        int ret = 0;
        int nfd = 0;
        ret = read(sfd[0], &nfd, sizeof(int));
        printf("ret  = %d\n", ret);
        printf("nfd = %d\n", nfd);

        char buf[64] = {0};
        read(nfd, buf, sizeof(buf));
        printf("buf = %s\n", buf);
    }
    return 0;
}

