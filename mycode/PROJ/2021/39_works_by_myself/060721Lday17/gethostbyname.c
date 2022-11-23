#include <func.h>

int main(int argc,char*argv[]){
    ARGS_CHECK(argc,2);
    struct hostent *hostInfo = gethostbyname(argv[1]);
    ERROR_CHECK(hostInfo, NULL, "gethostbyname");
    
    printf("host name: %s\n",hostInfo->h_name);
    for(int i = 0; hostInfo->h_aliases[i] != NULL; ++i){
        printf("one alia of alias list:%s \n",hostInfo->h_aliases[i]);
    }
    printf("host address type:%d \n",hostInfo->h_addrtype);
    printf("lenth of address:%d \n",hostInfo->h_length);
    char buf[1024] = { 0  };
    for(int i = 0; hostInfo->h_addr_list[i] !=  NULL; ++i){
        memset(buf, 0, sizeof(buf));
        inet_ntop(hostInfo->h_addrtype,hostInfo->h_addr_list[i], buf, sizeof(buf));
        printf("address list:%s \n",buf);
        /* memset(buf, 0, sizeof(buf)); */
        struct hostent *hostInfoByAddr = gethostbyaddr((void*)&hostInfo->h_addr_list[i], 4, AF_INET);
        ERROR_CHECK(hostInfoByAddr, NULL, "gethostbyaddr");
        printf("address list:%s \n",hostInfoByAddr->h_name);
    }
    return 0;
}

