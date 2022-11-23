#include"queue.h"
//#include<func.h>

bool isFull(Queue_t que){
    if(que.size==SIZE)
        return true;
}

bool isEmpty(Queue_t que){
    if(que.size==0)
        return true;
}

P Dequeue(Queue_t *que){
    if(isEmpty(*que)){
        printf("the queue is empty!\n");
        return (P)NULL;
    }
    P ret =que->Queue[que->Front];
    que->Front=++que->Front%SIZE;//修改结构体成员变量也需要传入指针

    que->size--;
    return ret;
}

void Enqueue(Queue_t* que,P val){
    if(isFull(*que)){
        printf("the queue is full\n");
        return;
    }
    que->Rear=++que->Rear%SIZE;//修改结构体成员变量也需要传入指针
    que->Queue[que->Rear]=val;
    //que->Rear++;
    que->size++;
}

P peek(Queue_t que){
    return que.Queue[que.Front];
}
Queue_t* QueCreate(){
    return (Queue_t*)calloc(1,sizeof(Queue_t));
}
void QueueDestroy(Queue_t *q){
    free(q);
}
