#include<func.h>

int main(int argc,char*argv[]){
    ARGS_CHECK(argc,3);
    DIR *pdir = opendir(argv[1]);
    struct dirent *dirent_buf;
    while((dirent_buf=readdir(pdir))!=NULL){
        printf("%c\n",(char)dirent_buf->d_type);
        if(strcmp(dirent_buf->d_name,argv[2])==0){
            printf("%s\n",getcwd(NULL,0));
        }
    }

    return 0;
}
