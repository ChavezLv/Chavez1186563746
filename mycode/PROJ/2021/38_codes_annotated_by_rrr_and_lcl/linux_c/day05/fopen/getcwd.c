#include <func.h>
int main()
{
    //char buf[12] = {0};
    //char *ret = getcwd(buf,sizeof(buf));
    //ERROR_CHECK(ret,NULL,"getcwd");
    //puts(buf);
    //puts(ret);
    printf("%s\n", getcwd(NULL,0));
    return 0;
}

