#include<func.h>
int main(int argc,char* argv[]){
    char* str[4]={"baidu.com","jd.com","qq.com","bilibili.com"};
    for(int i=0;i<4;i++){
    struct hostent *phost=gethostbyname(str[i]);
    printf("h_name:%s\n",phost->h_name);
    for(int i=0;phost->h_addr_list[i]!=0;++i){
    char buff[100]={0};
    inet_ntop(phost->h_addrtype,phost->h_addr_list[i],buff,100);
    printf("buff:%s\n",buff);
}
    printf("\n");
}}
