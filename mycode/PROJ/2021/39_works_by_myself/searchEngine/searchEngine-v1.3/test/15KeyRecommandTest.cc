#include "../include/KetRecommander.h"
int main(int argc,char *argv[])
{
    KetRecommander key(argv[1],argv[2],argv[3]);
    key.execute();
    return 0;
}

