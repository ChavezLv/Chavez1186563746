#include<func.h>
int main(void){
    int fd=open("file.txt",O_RDWR|O_CREAT|O_TRUNC,0666);
    ERROR_CHECK(fd,-1,"open");
    write(fd,"hello",5);
    pid_t pid=fork();
    if(pid==0){
        const char* msg="I am a baby\n";
        write(fd,msg,strlen(msg));
        return 0;
    }
    const char* msg="who's your daddy\n";
    write(fd,msg,strlen(msg));
    return 0;
}
