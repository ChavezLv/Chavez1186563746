#include<stdlib.h>
#include<sys/stat.h>
#include<stdio.h>
#include<unistd.h>
int main(int argc,char *argv[]){
	char *CurWorkDic=getcwd(NULL,0);
	puts(CurWorkDic);
	chdir(argv[1]);
	char* chcwd=getcwd(NULL,0);
        puts(chcwd);
	free(chcwd);
	free(CurWorkDic);
	return 0;
}
