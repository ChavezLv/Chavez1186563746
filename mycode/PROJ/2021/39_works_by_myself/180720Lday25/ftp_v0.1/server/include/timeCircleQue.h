#ifndef __TIMECIRCLEQUE_H__
#define __TIMECIRCLEQUE_H__

#include <sys/timerfd.h>
#include <time.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <unistd.h>

#define SIZE 3  //时间轮盘大小
#define HASHMAX 100//最大

/* const int MAXNUM = 20; */

typedef struct Fd_s{
    int fd;
    /* int pos; */
    struct Fd_s *pNext;
}Fd_t, *pFd_t;

typedef struct Node_s{
    Fd_t fdArr[HASHMAX];
    /* Pos_t pos[HASHMAX]; */
    /* struct Node_s *pNext; */
}Node_t, *pNode_t;

typedef struct TimeQue_s {
    Node_t node[SIZE];
	int front;
	int rear;
}TimeQue_t,*pTimeQue_t;

typedef struct Pos_s{
    int pos;
    int fd;
    struct Pos_s * pNext;
}Pos_t, *pPos_t;

typedef struct {
    Pos_t posArr[HASHMAX];
    TimeQue_t timeQue;    
}Map_t, *pMap_t;

int myHash(int newFd);
/* void init(pTimeQue_t pTimeQue); */
/* int  timeQueInit(pTimeQue_t pTimeQue, int *timefd) ; */
/* void timeQueIns(pTimeQue_t pTimeQue,int newFd); */
/* void timeQueRearChange(pTimeQue_t pTimeQue); */
/* void timeQueOut(pTimeQue_t pTimeQue); */
int  timeQueInit(pMap_t pMap, int *timefd) ;
void timeQueIns(pMap_t pMap, int newFd);
void timeQueRearChange(pMap_t pMap);
void timeQueOut(pMap_t pMap); 

int epollAddFd(int epfd, int fd);

#endif
