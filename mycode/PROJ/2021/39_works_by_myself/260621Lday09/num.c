#include <func.h>

int main(int argc,char*argv[]){
    ARGS_CHECK(argc,3);
    srand(time(NULL));
    int tmp = 0;
    int min = atoi(argv[1]);
    int max = atoi(argv[2]);
    while(1){
        tmp = rand()%100;
        if(tmp >= min &&tmp <= max){
            printf("%d\n",tmp);
            break;
        }
    }
    return 0;
}

