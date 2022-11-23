#include<func.h>
#include<stdio.h>

int DFSprint(char* path,int width);
int main(int argc,char *argv[]){
    ARGS_CHECK(argc,2);
    puts(argv[1]);
    DFSprint(argv[1],4);
    return 0;
}
int DFSprint(char* path,int width){
    char newPath[512] = {0};
    DIR *dirp = opendir(path);
    ERROR_CHECK(dirp,NULL,"opendir");
    struct dirent *buf;
    while((buf=readdir(dirp))!=NULL){
        if(strcmp(buf->d_name,".")==0||\
           strcmp(buf->d_name,"..")==0){
            continue;
        }
        printf("%*s%s\n",width,"",buf->d_name);
        //printf  *  通过width调节宽度
        if(buf->d_type==4){
            sprintf(newPath,"%s%s%s",path,"/",buf->d_name);
            DFSprint(newPath,width+4);
        }
    }
    closedir(dirp);
    return 0;
}
