#include<stdlib.h>
#include<sys/stat.h>
#include<stdio.h>
int main(int argc,char *argv[]){
	mode_t mode;
	sscanf(argv[2],"%o",&mode);
	int ret=chmod(argv[1],mode);
	(ret==0)?:printf("chmod error!");
	return 0;
}
