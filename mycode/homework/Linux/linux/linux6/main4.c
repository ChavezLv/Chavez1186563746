#include<stdlib.h>
#include<sys/stat.h>
#include<stdio.h>
#include<pwd.h>
#include<sys/types.h>
#include<dirent.h>
#include<string.h>
#include<unistd.h>
#include<time.h>
#include<grp.h>
int main(int argc,char *argv[]){
    DIR* pdir=opendir(argv[1]);
    struct ditrent* pdirent;
    struct stat pstat;
    while(pdirent=readdir(pdir)!=NULL){
         char *name = pdirent->d_name;
         char pathname[1024];
         sprintf(pathname,"%s/%s",argv[1],name);
         int ret=stat(pathname,&pstat);
         printf("%lu\t",pstat.st_ino);
         switch(pstat.st_mode&S_IFMT){
         case S_IFBLK: printf("b"); break;
         case S_IFCHR: printf("c"); break;
         case S_IFDIR: printf("d"); break;
         case S_IFIFO: printf("p"); break;
         case S_IFLNK: printf("l"); break;
         case S_IFREG: printf("-"); break;
         case S_IFSOCK:printf("s"); break;
         default:      printf("?"); break;
         }
         for(int i=0;i<3;i++){
            int m=(pstat.st_mode>>3*(2-i))&0x7;
            switch(m){
            case 0: printf("---"); break;
            case 1: printf("--x"); break;
            case 2: printf("-w-"); break;
            case 3: printf("-wx"); break;
            case 4: printf("r--"); break;
            case 5: printf("r-x"); break;
            case 6: printf("rw-"); break;
            case 7: printf("rwx"); break;              
         }}
        struct passwd* pwd=getpwuid(pstat.st_uid);
        struct group* grp=getgrgid(pstat.st_gid);
        printf("%ld %s %s %ld",
              pstat.st_nlink,
              pwd->pw_name,
              grp->gr_name,
              pstat.st_size );
        struct tm* ptime=localtime(&pstat.st_atim);
         printf("%d/%.2d/%.2d %.2d:%.2d:%.2d ",
               ptime->tm_year + 1900,
               ptime->tm_mon + 1,
               ptime->tm_mday,
               ptime->tm_hour,
               ptime->tm_min,
               ptime->tm_sec);
         puts(name);

    }
    closedir(pdir);
    return 0;
}
