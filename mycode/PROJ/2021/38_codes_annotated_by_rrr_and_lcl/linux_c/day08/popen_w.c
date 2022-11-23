#include <func.h>
int main()
{
    FILE *fp = popen("./sub","w");
    ERROR_CHECK(fp,NULL,"popen");
    fwrite("100000 99999999",1,15,fp);
    pclose(fp);
    return 0;
}

