#include <func.h>
int global = 1;
int main()
{
    pid_t pid = fork();
    int stack = 2;
    int *pHeap = (int *)malloc(sizeof(int));
    *pHeap = 3;
    if(pid == 0){
        printf("I am child, global = %d, stack = %d, *pHeap = %d\n", global,stack,*pHeap);
        ++global;
        ++stack;
        ++*pHeap;
        printf("I am child, global = %d, stack = %d, *pHeap = %d\n", global,stack,*pHeap);
    }
    else{
        printf("I am parent, global = %d, stack = %d, *pHeap = %d\n", global,stack,*pHeap);
        global += 2;
        stack += 2;
        *pHeap +=2;
        printf("I am parent, global = %d, stack = %d, *pHeap = %d\n", global,stack,*pHeap);
        sleep(1);
    }
    return 0;
}

