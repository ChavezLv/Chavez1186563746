#include <head.h>

int main(int argc, char** argv)
{
    struct hostent* pHost = gethostbyname(argv[1]);
    if(NULL == pHost){
        return -1;
    }

    printf("host name = %s\n", pHost->h_name);

    for(int i = 0; pHost->h_aliases[i] != NULL; ++i){
        printf("h_aliases = %s\n", pHost->h_aliases[i]);
    }
    
    printf("addrtype = %d\n", pHost->h_addrtype);
    printf("addrlen = %d\n", pHost->h_length);

    char buf[64] = {0};
    for(int i = 0; pHost->h_addr_list[i] != NULL; ++i){
        memset(buf, 0, sizeof(buf));
        inet_ntop(pHost->h_addrtype, pHost->h_addr_list[i], buf, sizeof(buf));
        printf("buf = %s\n", buf);
    }
    return 0;
}

