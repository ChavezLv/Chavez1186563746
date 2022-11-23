#include <func.h>
int main(int argc, char *argv[])
{
    // ./chdir dir
    ARGS_CHECK(argc,2);
    printf("Before chdir, cwd = %s\n", getcwd(NULL,0));
    int ret = chdir(argv[1]);
    ERROR_CHECK(ret,-1,"chdir");
    printf("After chdir, cwd = %s\n", getcwd(NULL,0));
    return 0;
}

