#include <func.h>
int main()
{
    FILE *fp = popen("./print", "r");
    ERROR_CHECK(fp,NULL,"popen");
    char buf[128] = {0};
    fread(buf,1,sizeof(buf),fp);
    printf("msg from pipe!\n");
    puts(buf);
    pclose(fp);
    return 0;
}

