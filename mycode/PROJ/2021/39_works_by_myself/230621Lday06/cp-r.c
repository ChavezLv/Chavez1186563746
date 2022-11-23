#include<func.h>

int cpFile(char* from, char* to);
int cpDir(char* from,char* to);
int main(int argc, char* argv[]) {
    //ARGS_CHECK(argc, 3);
    if(argc==3){
        cpFile(argv[1], argv[2]);
    }else if(argc==4){
        //cp-r  dir1  dir2 -r   
        if(strcmp(argv[3],"-r")==0){
            cpDir(argv[1],argv[2]);
        }
        //cp-r -r dir1 dir2
        else if(strcmp(argv[1],"-r")==0){
            cpDir(argv[2],argv[3]);
        }
    }
    return 0;
}
int cpFile(char* from,char*to) {
    int fdr = open(from, O_RDONLY);
    ERROR_CHECK(fdr,-1,"open fdr");
    int fdw = open(to,O_WRONLY|O_CREAT|O_TRUNC);
    ERROR_CHECK(fdw,-1,"open fdw");
    chmod(to,0664);
    char buf[4096] = {0};
    while(1){
        int ret = read(fdr,buf,sizeof(buf));
        ERROR_CHECK(ret,-1,"read");
        if(ret==0){
            break;
        }
        write(fdw,buf,ret);
    }
    return 0;
}
int cpDir(char* from,char* to){
     DIR *dir_from = opendir(from);
     ERROR_CHECK(dir_from,NULL,"opendir");
     DIR *dir_to = opendir(to);
     if(dir_to == NULL){
        mkdir(to,0775);
        chmod(to,0775);
        dir_to = opendir(to);
        ERROR_CHECK(dir_to,NULL,"opendir");
     }
     struct dirent *dirent_from;
     char npath_from[512] = {0};
     char npath_to[512] = {0};
     while((dirent_from = readdir(dir_from))!=NULL){
        if(strcmp(dirent_from->d_name,".")==0||\
           strcmp(dirent_from->d_name,"..")==0){
           continue;
        }
        sprintf(npath_from,"%s%s%s",from,"/",dirent_from->d_name);
        sprintf(npath_to,"%s%s%s",to,"/",dirent_from->d_name);
        if(dirent_from->d_type==4){
            printf("%s\n",npath_from);
            cpDir(npath_from,npath_to);
        }else{
            cpFile(npath_from,npath_to);
        }
     }
     return 0;
}

