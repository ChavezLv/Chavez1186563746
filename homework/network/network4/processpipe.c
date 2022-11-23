#include<func.h>
//#include <head.h>

int main(int argc, char *argv[])
{
	int fds[2];
	pipe(fds);
	printf("fds[0]:%d, fds[1]:%d\n", fds[0], fds[1]);

	int ret = fork();
	if(ret > 0) {
		//父进程
		close(fds[0]);
		int fileFd = open("file", O_RDWR);
		printf("fileFd:%d\n", fileFd);
		ERROR_CHECK(fileFd, -1, "open");

		write(fds[1], &fileFd, sizeof(fileFd));
		wait(NULL);
	} else {
		//子进程
		close(fds[1]);
		int childfd = 0;
		read(fds[0], &childfd, sizeof(childfd));
		printf("childfd:%d\n", childfd);

		char buff[100] = {0};
		read(childfd, buff, sizeof(buff));//childfd只是自定义的一个整形数据,并没有在内核对应的文件
        puts(buff);
	}
	return 0;
}
