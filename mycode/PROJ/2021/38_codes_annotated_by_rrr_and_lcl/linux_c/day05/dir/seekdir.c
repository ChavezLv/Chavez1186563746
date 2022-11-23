#include <func.h>
int main(int argc, char *argv[])
{
    ARGS_CHECK(argc,2);
    DIR *dirp = opendir(argv[1]);
    ERROR_CHECK(dirp,NULL,"opendir");
    struct dirent *pdirent;
    long loc;
    while((pdirent = readdir(dirp)) != NULL){
        printf("ino = %ld, reclen = %d, type = %d, name = %s\n",
               pdirent->d_ino,
               pdirent->d_reclen,
               pdirent->d_type,
               pdirent->d_name);
        if(strcmp(pdirent->d_name,"dir1") == 0){
            loc = telldir(dirp);
        }
    }
    puts("-------------------------------");
    seekdir(dirp,loc);
    pdirent = readdir(dirp);
    if(pdirent != NULL){//回到了最后，再次read时，dirent是空的
        printf("ino = %ld, reclen = %d, type = %d, name = %s\n",
               pdirent->d_ino,
               pdirent->d_reclen,
               pdirent->d_type,
               pdirent->d_name);
    }
    closedir(dirp);
    return 0;
}

