#ifndef __queue_H__
#define __queue_H__ti
#include<stdio.h>
#include<stdbool.h>
#include<head.h>
#define SIZE 10
typedef struct TreeNode_s TreeNode,*P;

typedef struct Queue_Node_s{
    int Rear;
    int Front;
    int size;
    P Queue[SIZE];//队列元素
}Queue_t;

bool isFull(Queue_t que);
bool isEmpty(Queue_t que);
void Enqueue(Queue_t *que,P val);
P Dequeue(Queue_t* que);
P peek(Queue_t que);

Queue_t* QueCreate();//为队列开辟堆空间
void QueDestroy(Queue_t* q);
#endif
