#include<func.h>
typedef struct stu_s{
    int num;
    int score;
    char name[20];
}Stu_t,*pStu_t;
int main(int argc,char* argv[]){
    Stu_t s[3]={{1,60,"dog"},{2,83,"boy"},{3,99,"girl"}};
    ARGS_CHECK(argc,2);
    int fd = open(argv[1],O_RDWR);
    ERROR_CHECK(fd,-1,"open");
    char buf[1024] = {0};
    for(int  j=0;j<3;++j){
        memset(buf,0,sizeof(strlen(buf)));
        sprintf(buf,"%s %d %d\n",s[j].name,s[j].num,s[j].score);
        write(fd,buf,sizeof(strlen(buf)));
        write(fd,"\n",1);
    }
    lseek(fd,0,SEEK_SET);
    memset(buf,0,sizeof(buf));
    read(fd,buf,sizeof(buf));
    puts(buf);
    return 0;
}
