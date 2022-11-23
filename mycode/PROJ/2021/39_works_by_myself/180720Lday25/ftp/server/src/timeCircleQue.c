#include "../include/head.h"
#include "../include/timeCircleQue.h"

int myHash(int newFd){
    return newFd % HASHMAX;
}

/* int  timeQueInit(pTimeQue_t pTimeQue, int *timefd) { */
int  timeQueInit(pMap_t pMap, int *timefd){
    memset(pMap, 0, sizeof(Map_t));
    struct itimerspec new_value;
    struct timespec now;

    int ret = clock_gettime(CLOCK_REALTIME, &now);//获取时钟时间
    /* assert(ret != -1); */


    new_value.it_value.tv_sec = 1; //第一次到期的时间
    new_value.it_value.tv_nsec = now.tv_nsec; 

    new_value.it_interval.tv_sec = 1;      //之后每次到期的时间间隔
    new_value.it_interval.tv_nsec = 0;

    *timefd = timerfd_create(CLOCK_REALTIME, TFD_NONBLOCK); // 构建了一个定时器
    /* assert(timefd != -1); */

    ret = timerfd_settime(*timefd, 0, &new_value, NULL);//启动定时器

    return 0;
}

/* void timeQueRearChange(pTimeQue_t pTimeQue){ */
void timeQueRearChange(pMap_t pMap){
    pTimeQue_t pTimeQue = &pMap->timeQue;
    pTimeQue->rear = (pTimeQue->rear + 1) % SIZE; 

}

/* void timeQueIns(pMap_t pMap, int newFd){ */
void timeQueIns(pMap_t pMap, int newFd){
    printf("ins: %d\n",newFd);
    pTimeQue_t pTimeQue = &pMap->timeQue;
    int nodeIndex = myHash(newFd);
    int queIndex = 0;
    //删除
    int preIndex = pMap->posArr[nodeIndex].pos; 
    if(pMap->posArr[nodeIndex].fd == newFd){
        //如果fd已经存在
        pMap->timeQue.node[preIndex].fdArr[nodeIndex].fd = 0; 
    }else if(pMap->posArr[nodeIndex].fd != 0){
        //如果是hash冲突情况
        pPos_t pCur = &pMap->posArr[nodeIndex];
        while(pCur->pNext != NULL){
            if(pCur->fd == newFd){
                pCur->fd = 0;
            }
            pCur = pCur->pNext;
        }
    }
    /* for(int i = 0; i < SIZE; ++i){ */
    /*     if(pTimeQue->node[i].fdArr[nodeIndex].fd == newFd){ */
    /*         pTimeQue->node[i].fdArr[nodeIndex].fd =  0; */
    /*     }else{ */
    /*         pFd_t pCur = &pTimeQue->node[i].fdArr[nodeIndex]; */
    /*         while(pCur->pNext != NULL){ */
    /*             if(pCur->pNext->fd == newFd){ */
    /*                 pCur->pNext->fd = 0; */
    /*                 break; */
    /*             } */
    /*             pCur = pCur->pNext; */
    /*         } */
    /*     } */
    /* } */
    //插入上一格
    if(0 == pTimeQue->rear){
        queIndex = SIZE - 1;

    }else{
        queIndex = pTimeQue->rear - 1;

    }
    //哈希冲突
    if(0 == pTimeQue->node[queIndex].fdArr[nodeIndex].fd){
        pTimeQue->node[queIndex].fdArr[nodeIndex].fd = newFd;
        pMap->posArr[queIndex].pos = queIndex;
        /* pTimeQue->node[queIndex].fdArr[nodeIndex].pos = queIndex; */
    }else{
        pFd_t pNew = (pFd_t) calloc(1, sizeof(Fd_t));
        pNew->fd = newFd;
        pPos_t pPos = (pPos_t) calloc(1, sizeof(Pos_t));
        pPos->pos = queIndex;
        /* pNew->pos = queIndex; */
        
        pFd_t pCur = &pTimeQue->node[queIndex].fdArr[nodeIndex];
        pPos_t pTmp = &pMap->posArr[queIndex];
        while(pCur->pNext){
            pCur = pCur->pNext;
        }
        pCur->pNext = pNew;
        pTmp->pNext = pPos;
    }

}

/* void timeQueOut(pTimeQue_t pTimeQue) { */
void timeQueOut(pMap_t pMap){ 
    pTimeQue_t pTimeQue = &pMap->timeQue;
    Node_t node = pTimeQue->node[pTimeQue->rear];
    for(int i = 0; i < HASHMAX; ++i){
        if(node.fdArr[i].pNext == NULL){
            close(node.fdArr[i].fd);
        }else{
            pFd_t pCur = &node.fdArr[i];
            while(pCur->pNext){
                close(pCur->fd);
                pCur = pCur->pNext;
            }
        }
    }

}


#ifdef DEBUG
int main() {
    TimeQueue_t queue;

    init(&queue);
    for (int i = 0; i < SIZE; ++i) {
        inQueue(&queue, i);

    }
    for (int i = 0; i < SIZE; ++i) {
        printf("%-3d",deQueue(&queue));

    }
    printf("\n");
    return 0;

}
#endif
