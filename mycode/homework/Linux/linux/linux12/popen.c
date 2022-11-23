#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<pthread.h>
int main(){
    FILE* fp1=popen("./echoall","r");
    FILE* fp2=fopen("a.txt","w");
    char buf[1024];
    while(fgets(buf,sizeof(buf),fp1)!=NULL){
        fputs(buf,fp2);
}
    pclose(fp1);
    fclose(fp2);
    return 0;
}
