 ///
 /// @file    taskqueue.c
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2022-10-07 09:54:56
 ///
 

#include "taskqueue.h"
#include "head.h"

void queueInit(task_queue_t * que)
{
	if(que) {
		que->pFront = NULL;
		que->pRear = NULL;
		que->queSize = 0;
		que->exitFlag = 0;
		int ret = pthread_mutex_init(&que->mutex, NULL);
		THREAD_ERROR_CHECK(ret, "phtread_mutex_init");
		
		ret = pthread_cond_init(&que->cond, NULL);
		THREAD_ERROR_CHECK(ret, "phtread_cond_init");
	}
}

void queueDestroy(task_queue_t * que)
{
	if(que) {
		int ret = pthread_mutex_destroy(&que->mutex);
		THREAD_ERROR_CHECK(ret, "pthread_mutex_destroy");
		ret = pthread_cond_destroy(&que->cond);
		THREAD_ERROR_CHECK(ret, "pthread_cond_destroy");
	}
}

int queueIsEmpty(task_queue_t * que)
{
	return que->queSize == 0;
}

int getTaskSize(task_queue_t * que)
{
	return que->queSize;
}

//主线程中调用, 作为生产者被调用
void taskEnqueue(task_queue_t * que, int peerfd)
{
	//加锁
	pthread_mutex_lock(&que->mutex);
	task_t * pNewtask = (task_t*)calloc(1, sizeof(task_t));
	pNewtask->peerfd = peerfd;
	pNewtask->pnext = NULL;
	if(getTaskSize(que) == 0) {
		que->pFront = que->pRear = pNewtask;
	}else {
		que->pRear->pnext = pNewtask;
		que->pRear = pNewtask;
	}
	que->queSize++;
	//解锁
	pthread_mutex_unlock(&que->mutex);
	//通知消费者去任务节点
	pthread_cond_signal(&que->cond);
}

//子线程中调用, 作为消费者使用
int taskDequeue(task_queue_t * que)
{
	//加锁
	pthread_mutex_lock(&que->mutex);

	//使用while为了防止虚假唤醒
	while(!que->exitFlag && queueIsEmpty(que)) {
		//队列为空，不能取节点数据,进入等待状态
		pthread_cond_wait(&que->cond, &que->mutex);
	}

	if(!que->exitFlag) {
		//元素出队操作
		task_t * pDeleteNode = que->pFront;
		int peerfd = pDeleteNode->peerfd;
		if(getTaskSize(que) > 1) {
			que->pFront = que->pFront->pnext;
		} else {
			que->pFront = que->pRear = NULL;
		}
		que->queSize--;
		//解锁
		pthread_mutex_unlock(&que->mutex);
		free(pDeleteNode);
		return peerfd;
	} else {
		pthread_mutex_unlock(&que->mutex);
		return -1;
	}
}

void queueWakeup(task_queue_t* que)
{
	que->exitFlag = 1;
	pthread_cond_broadcast(&que->cond);
}

