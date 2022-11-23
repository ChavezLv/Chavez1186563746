#include <func.h>
#define N 10000
#define M 1000
#define SWAP(lhs,rhs) {int tmp = 0;tmp = rhs;rhs = lhs;lhs= tmp;}
void adiustMaxHeap(int *arr,int pos,int len){
    //pos is max father node's index 
    int dad = pos;
    int son = 2*dad+1;
    while(son<len){
        if(son+1<len&&arr[son]<arr[son+1]){
            ++son;
        }
        if(arr[son]>arr[dad]){
            SWAP(arr[son],arr[dad]);
            dad = son;
            son = 2*dad+1;
        }else{
            break;
        }
    }
}
void heapSort(int *arr){
    //N  N/2  N/2-1
    for(int i = N/2 - 1;i >= 0;--i){
        adiustMaxHeap(arr,i,N);
    }
    SWAP(arr[0],arr[N-1]);
    for(int i = N-1;i>=2;--i){
        adiustMaxHeap(arr,0,i);
        SWAP(arr[0],arr[i-1]);
    }
}

int main(int argc,char*argv[]){
    ARGS_CHECK(argc,2);
    int fd = open(argv[1],O_TRUNC|O_RDWR);
    //time_t tm;
    srand(time(NULL));
    char buf[10] = {0};
    int arr[N] = {0};
    for(int i = 0;i<N;++i){
        memset(buf,0,sizeof(buf));
        int tmp = rand()%M;
        sprintf(buf,"%d ",tmp);
        arr[i] = tmp;
        write(fd,buf,strlen(buf));
    }
    heapSort(arr);
    int fdw = open("after_sort",O_CREAT|O_WRONLY,0664); 
    int cnt = 0;
    for(int i = 0;i<N;++i,++cnt){
        memset(buf,0,sizeof(buf));
        sprintf(buf,"%d ",arr[i]);
        if(cnt>=10){
            sprintf(buf,"\n");
            cnt = 0;
        }
        write(fdw,buf,strlen(buf));
    }
    return 0;
}

