#include "queue.h"
#include <stdio.h>
#include <string.h>

// 初始化队列
void InitQueue(Queue *q){
	q->front=0;
	q->rear=0;
} 

// 判队空
int IsQueueEmpty(const Queue *q){
	return q->front==q->rear;
} 

// 判队满
int IsQueueFull(Queue *q){
	return (q->rear+1)%QUEUE_MAX_SIZE==q->front;
} 

// 入队
int Enqueue(Queue *q,const char *value){
	if(IsQueueFull(q)){
		printf("队列已满，无法入队\n");
		return 0;
	}
	strcpy(q->elements[q->rear].data,value);
	q->rear=(q->rear+1)%QUEUE_MAX_SIZE;
	return 1;
} 

// 出队
int Dequeue(Queue *q,char *value){
	if(IsQueueEmpty(q)){
		printf("队列为空，无法出队\n");
		return 0;
	}
	strcpy(value,q->elements[q->front].data);
	q->front=(q->front+1)%QUEUE_MAX_SIZE;
	return 1;
} 

// 取队头元素
int Front(Queue *q,char *value){
	if(IsQueueEmpty(q)){
		printf("队列为空\n");
		return 0;
	} 
	strcpy(value,q->elements[q->front].data);
	return 1;
} 

// 销毁队列
void DestroyQueue(Queue *q){
	q->front=0;
	q->rear=0;
	int i;
	for(i=0;i<QUEUE_MAX_SIZE;i++)
		memset(q->elements[i].data,0,sizeof(q->elements[i].data));
} 
