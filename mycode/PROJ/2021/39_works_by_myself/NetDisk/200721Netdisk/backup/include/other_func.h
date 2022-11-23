#ifndef __OTHER_FUNC_H__
#define __OTHER_FUNC_H__
#include"head.h"
#include"../include/work_Queue.h"
#include"../include/pthread_Pool.h"

int init_Pool_Queue(pPool_t pPool, pQueue_t Queue, int threadNum);
int tcp_init(char *ip, char *port);
int recycle(pPool_t pPool);
#endif
