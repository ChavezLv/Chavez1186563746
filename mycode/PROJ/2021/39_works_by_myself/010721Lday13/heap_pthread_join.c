#include <func.h>

void *threadFunc(void* p){
    char *str = (char*)calloc(1,sizeof(10));
    strcpy(str,"hello");

    pthread_exit((void*)str);
}
int main(int argc,char*argv[]){
    int ret;
    pthread_t thid;
    ret = pthread_create(&thid,NULL,threadFunc, NULL);
    THREAD_ERROR_CHECK(ret,"pthread_create");


    char *retStr = NULL;
    ret = pthread_join(thid,(void**)&retStr);
    THREAD_ERROR_CHECK(ret,"pthread_cread");
    
    printf("pthread_join return val str:%s.\n",retStr);
    free(retStr);
    retStr = NULL;
    return 0;
}

