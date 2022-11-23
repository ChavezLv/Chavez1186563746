#include<stdlib.h>
#include<stdio.h>
int main(int argc,char *argv[]){
	FILE* fp=fopen(argv[1],"a+");
	for(int i=0;i<3;i++){
		char buffer[1024];
		fgets(buffer,1024,fp);//fgets读取n-1个或者读取到换行符时或者到达文件末尾则停止读取;
		puts(buffer);//遇到'/0'停止输出
		printf("当前文件流位置:%ld\n",ftell(fp));
	}
	fputs("fireman\n",fp);
	printf("写入后的文件流位置:%ld\n",ftell(fp));
	fclose(fp);
	return 0;
}
