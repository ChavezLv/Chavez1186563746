#include<func.h>

int ls_l(char *path);
char filetype(mode_t st_mode);
char *filemode(mode_t st_mode);

int main(int argc,char*argv[]){
    /*ls_l .*/
    ARGS_CHECK(argc,2);
    ls_l(argv[1]);
    return 0;
}

int ls_l(char *path){
    DIR *dirp = opendir(path);
    ERROR_CHECK(dirp,NULL,"opendir");
    struct dirent *dirent_s = NULL;
    struct stat stat_s = {0};
    //char mode_buf[9] = {0};
    char buf[512] = {0};
    int total = 0;
    while((dirent_s = readdir(dirp))!=NULL){
        sprintf(buf,"%s%s%s",path,"/",dirent_s->d_name);
        int ret = stat(buf,&stat_s);
        ERROR_CHECK(ret,-1,"stat");
        if(strcmp(dirent_s->d_name,".")==0||\
           strcmp(dirent_s->d_name,"..")==0){
            total+=stat_s.st_size/1024;
            continue;
        }
        //printf("%hhu%o%-10lu%-10s%-10s%-10ld%-15s\n",dirent_s->d_type,stat_s.st_mode,
        //       stat_s.st_nlink,getpwuid(stat_s.st_uid)->pw_name,getgrgid(stat_s.st_gid)->gr_name,
        //       stat_s.st_blocks,dirent_s->d_name); 
        printf("%c%s %3ld %-10s %-10s %5ld %d%s  %d %02d%s%02d %-15s\n",filetype(stat_s.st_mode),filemode(stat_s.st_mode),\
               stat_s.st_nlink,getpwuid(stat_s.st_uid)->pw_name,getgrgid(stat_s.st_gid)->gr_name,\
               stat_s.st_size,localtime(&stat_s.st_mtime)->tm_mon+1,"月",\
               localtime(&stat_s.st_mtime)->tm_mday,\
               localtime(&stat_s.st_mtime)->tm_hour,":",localtime(&stat_s.st_mtime)->tm_min,dirent_s->d_name); 
        if(stat_s.st_size%1024==0){
            total+=stat_s.st_size/1024;
        }else{
            total = stat_s.st_size/1024+1;
        }
    }
    printf("总用量:%d\n",total);
    return 0;
}
char *filemode(mode_t st_mode){
    char *buf = (char*)calloc(1,sizeof(char)*10);
    sprintf(buf,"%o",st_mode);
    char *mode_buf =(char*)calloc(1,sizeof(char)*10);
    for(int i  = strlen(buf)-1,j=8;i>= (int)strlen(buf)-3;--i,j-=3){
        if(buf[i]=='7'){
            mode_buf[j] = 'x';
            mode_buf[j-1] = 'w';
            mode_buf[j-2] = 'r';
        }
        if(buf[i]=='6'){
            mode_buf[j] = '-';
            mode_buf[j-1] = 'w';
            mode_buf[j-2] = 'r';
        }
        if(buf[i]=='4'){
            mode_buf[j] = '-';
            mode_buf[j-1] = '-';
            mode_buf[j-2] = 'r';
        }
        if(buf[i]=='0'){
            mode_buf[j] = '-';
            mode_buf[j-1] = '-';
            mode_buf[j-2] = '-';
        }
        if(buf[i]=='1'){
            mode_buf[j] = 'x';
            mode_buf[j-1] = '-';
            mode_buf[j-2] = '-';
        }
        if(buf[i]=='2'){
            mode_buf[j] = '-';
            mode_buf[j-1] = 'w';
            mode_buf[j-2] = '-';
        }
        if(buf[i]=='3'){
            mode_buf[j] = 'x';
            mode_buf[j-1] = 'w';
            mode_buf[j-2] = '-';
        }
        if(buf[i]=='5'){
            mode_buf[j] = 'x';
            mode_buf[j-1] = '-';
            mode_buf[j-2] = 'r';
        }
    }
    return mode_buf;
}
char filetype(mode_t st_mode){
    switch (st_mode & S_IFMT) {
    case S_IFBLK:  return 'b';  break;
    case S_IFCHR:  return 'c';  break;
    case S_IFDIR:  return 'd';  break;
    case S_IFIFO:  return 'p';  break;
    case S_IFLNK:  return 'l';  break;
    case S_IFREG:  return '-';  break;
    case S_IFSOCK: return 's';  break;
    default:       return '-';  break;
    }
}

//char *getMon(int tm_mon){
//    switch(tm_mon)
//    {
//    case 0:
//    return "Jan";
//    break;
//    case 1:
//    return "Feb";
//    break;
//    case 2:
//    return "Mar";
//    break;
//    case 3:
//    return "Apr";
//    break;
//    case 4:
//    return "May";
//    break;
//    case 5:
//    return "Jun";
//    break;
//    case 6:
//    return "Jul";
//    break;
//    case 7:
//    return "Aug";
//    break;
//    case 8:
//    return "Sep";
//    break;
//    case 9:
//    return "Oct";
//    break;
//    case 10:
//    return "Nov";
//    break;
//    case 11:
//    return "Dec";
//    break;
//    default:
//    break;
//    }
//    return 0;
//}
