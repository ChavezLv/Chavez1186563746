 ///
 /// @file    threadpool.c
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2022-10-07 10:50:52
 ///
 
#include "threadpool.h"
//线程池初始化
void threadpoolInit(threadpool_t * pthreadpool, int threadNum)
{
	if(pthreadpool) {
		pthreadpool->threadNumber = threadNum;
		pthreadpool->threads = (pthread_t*)calloc(threadNum, sizeof(pthread_t));
		queueInit(&pthreadpool->queue);
	}
}

void threadpoolDestroy(threadpool_t * pthreadpool)
{
	if(pthreadpool) {
		free(pthreadpool->threads);
		queueDestroy(&pthreadpool->queue);
	}
}

void threadpoolStart(threadpool_t * pthreadpool)
{
	//让N个子线程运行起来
	for(int i = 0; i < pthreadpool->threadNumber; ++i) {
		int ret = pthread_create(&pthreadpool->threads[i], NULL, threadFunc, pthreadpool);	
		THREAD_ERROR_CHECK(ret, "pthread_create");
	}
}

void * threadFunc(void * arg)
{
	//首先获取线程池的任务队列
	threadpool_t * pthreadpool = (threadpool_t*)arg;
	
	while(1) {
		if(pthreadpool) {
			//任务队列中有任务时，就直接返回
			//没有任务时，子线程就阻塞在这里
			int peerfd = taskDequeue(&pthreadpool->queue);
			if(peerfd > 0){
				transferFile(peerfd);
			} else {
				break;
			}
		}
	}
	return NULL;
}

void threadpoolStop(threadpool_t * pthreadpool)
{
	while(!queueIsEmpty(&pthreadpool->queue)) {
		sleep(1);//希望队列中的任务都被执行完毕后再退出
	}
	queueWakeup(&pthreadpool->queue);
	for(int i = 0; i < pthreadpool->threadNumber; ++i) {
		pthread_join(pthreadpool->threads[i], NULL);
	}
}
