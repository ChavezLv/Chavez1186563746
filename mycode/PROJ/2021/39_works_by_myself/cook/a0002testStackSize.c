/*================================================================
*   Copyright (C) 2021 anonymous All rights reserved.
*   
*   @file       ：a0002testStackSize.c
*   @author     ：anonymous
*   @date       ：2021年09月11日 19:25:49
*   @description：
*
================================================================*/


#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <pthread.h>
#include <unistd.h>
void test(int i)
{
    char temp[1024*1024] = {0};// 申请1M

    temp[0] = i;
    temp[0] ++;
    printf("%s %d num = %d!\r\n", __FUNCTION__, __LINE__, temp[0]);
    test(temp[0]);
}

void * pthreadFunc(void *){
    /* printf("%ld, %s, %d\n", pthread_self(), __FUNCTION__, __LINE__); */
    printf("%lu, %s, %d\n", pthread_self(), __FUNCTION__, __LINE__);
    test(0);
    
    pthread_exit(NULL);
}
int main(void)
{

    pthread_t pthid;
    if(0 != pthread_create(&pthid, NULL, pthreadFunc, NULL)){
        exit(-1);
    }

    usleep(100000);
    //方式一:
    /* test(0); */
    //方式二:
    /* char temp1[1024*1024] = {0};// 申请1M */
    /* char temp2[1024*1024] = {0};// 申请1M */
    /* char temp3[1024*1024] = {0};// 申请1M */
    /* char temp4[1024*1024] = {0};// 申请1M */
    /* char temp5[1024*1024] = {0};// 申请1M */
    /* char temp6[1024*1024] = {0};// 申请1M */
    /* char temp7[1024*1024] = {0};// 申请1M */
    /* char temp8[1024*1024] = {0};// 申请1M */
    //方式三:
    /* struct rlimit  rlimi; */
    /* int limit = getrlimit(RLIMIT_STACK, &rlimi);//limit = 0; */
    /* printf("%ld\n", rlimi.rlim_max);// -1 */
    /* printf("%ld\n", rlimi.rlim_cur/1024);//8388608/ 1024 = 8192 */
    return 0;
}
