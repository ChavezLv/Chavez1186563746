#include <func.h>
int main(int argc, char *argv[])
{
    ARGS_CHECK(argc,2);
    DIR *dirp = opendir(argv[1]);
    ERROR_CHECK(dirp,NULL,"opendir");
    chdir(argv[1]);//为了文件名可以直接当路径使用
    struct dirent *pdirent;
    struct stat statbuf;
    while((pdirent = readdir(dirp)) != NULL){
        int ret = stat(pdirent->d_name,&statbuf);
        ERROR_CHECK(ret,-1,"stat");
        printf("%6o %ld %s %s %6ld %15s %s",
               statbuf.st_mode, 
               statbuf.st_nlink,
               getpwuid(statbuf.st_uid)->pw_name,
               getgrgid(statbuf.st_gid)->gr_name,
               statbuf.st_size,
               pdirent->d_name,
               ctime(&statbuf.st_mtime)
              );
    }
    closedir(dirp);
    return 0;
}

