#include<stdlib.h>
#include<sys/stat.h>
#include<stdio.h>
#include<sys/types.h>
#include<dirent.h>
#include<string.h>
#include<unistd.h>
int main(int argc,char *argv[]){
	char path[1024];
	sprintf(path,"%s/%s",argv[1],argv[2]);//sprintf(path,argv[1],"/",argv[2]);这样写无法破解argv[2]
	DIR* pdir=opendir(argv[1]);
	struct dirent* pdirent;
	while(pdirent=readdir(pdir)){
		if(strcmp(pdirent->d_name,argv[2])==0){
			puts(path);
			closedir(pdir);
			return 0;
		}
	}
	printf("该文件不存在\n");
	closedir(pdir);
	return 0;
}
