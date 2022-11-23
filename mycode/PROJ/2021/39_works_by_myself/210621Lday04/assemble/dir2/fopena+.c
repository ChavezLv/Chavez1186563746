#include<func.h>
#include<stdio.h>
int main(int argc,char *argv[]){
    ARGS_CHECK(argc,2);
    FILE *fp = fopen(argv[1],"a+");
    if(fp==NULL){
        printf("error\n");
        return 0;
    }
    //ERROR_CHECK(fp,NULL,fopen);
    char buf[512] = {0};
    for(int i = 0;i<3;++i){
        fgets(buf,sizeof(buf),fp);
        printf("%s",buf);
        printf("%ld\n",ftell(fp));
        memset(buf,0,sizeof(buf));
    }
    fwrite("good",1,sizeof("goof"),fp);
    return 0;
}
