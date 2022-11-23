#include <func.h>

void* threadFunc(void* pp){//这里注意void* 需要返回return NULL
    /* long* p = (long *)calloc(1,sizeof(int)); */
    /* *p = 4; */
    long val = 4;
    pthread_exit((void*)val);//注意这里val不需要去地址
}

int main(int argc,char*argv[]){
    int ret;
    pthread_t thid;//这里不要写成int
    ret = pthread_create(&thid,NULL,threadFunc,NULL);
    THREAD_ERROR_CHECK(ret,"pthread_create");

    long retVal = 0;
    ret = pthread_join(thid,(void**)&retVal);
    THREAD_ERROR_CHECK(ret,"pthread_create");
    printf("%d.\n",(int)retVal);
    return 0;
}

