#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<pthread.h>
#define Size 6
int test[Size]={1,5,9,4,5,0};
int comp(const void* a,const void*b){
    return (*(int*) b-*(int*)a);
}
int main(){
    printf("before:");
    for(int i=0;i<Size;++i){
        printf("%d,",test[i]);
    }
    qsort(test,6,sizeof(int),comp);
    printf("\nafter:");
    for(int i=0;i<Size;++i){
        printf("%d,",test[i]);
    }
    printf("\n");    
}
