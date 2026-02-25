#ifndef QUEUE_H
#define QUEUE_H

#define QUEUE_MAX_SIZE 20  // 队列最大长度

// 队列节点数据类型 
typedef struct{
	char data[QUEUE_MAX_SIZE];
}QueueNode;

// 队列结构定义 
typedef struct{
	QueueNode elements[QUEUE_MAX_SIZE];  // 队列存储空间
	int front;  // 队头指针 
	int rear;   // 队尾指针
}Queue;  

// 初始化队列 
void InitQueue(Queue *q);
// 判队空 
int IsQueueEmpty(const Queue *q);
// 判队满 
int IsQueueFull(Queue *q);
// 入队 
int Enqueue(Queue *q, const char *value);
// 出队 
int Dequeue(Queue *q, char *value);
// 取队头元素 
int Front(Queue *q, char *value);
// 销毁队列
void DestroyQueue(Queue *q);

#endif 
