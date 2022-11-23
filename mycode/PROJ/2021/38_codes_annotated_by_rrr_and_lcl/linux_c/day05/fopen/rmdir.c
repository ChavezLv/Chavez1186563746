#include <func.h>
int main(int argc, char *argv[])
{
    ARGS_CHECK(argc,2);
    int ret = rmdir(argv[1]);//只能删除空目录
    ERROR_CHECK(ret,-1,"rmdir");
    return 0;
}

