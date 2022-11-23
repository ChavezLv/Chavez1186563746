#include <func.h>
int main(int argc, char *argv[])
{
    ARGS_CHECK(argc,2);
    int fdr = open(argv[1], O_RDONLY);
    while(1);
    return 0;
}

