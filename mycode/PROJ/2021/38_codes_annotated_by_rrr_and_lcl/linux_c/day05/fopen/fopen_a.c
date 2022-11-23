#include <func.h>
int main(int argc, char *argv[])
{
    ARGS_CHECK(argc, 2);
    FILE *fp = fopen(argv[1],"a+");
    ERROR_CHECK(fp,NULL,"fopen");
    char buf[6] = {0};
    fread(buf,1,sizeof(buf)-1,fp);
    puts(buf);
    fwrite("how are you",1,11,fp);
    fseek(fp,0,SEEK_SET);
    memset(buf,0,sizeof(buf));
    fread(buf,1,sizeof(buf)-1,fp);
    puts(buf);
    fwrite("how are you",1,11,fp);
    fclose(fp);
    return 0;
}

