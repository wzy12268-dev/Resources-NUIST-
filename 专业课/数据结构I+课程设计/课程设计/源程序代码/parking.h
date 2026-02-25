#ifndef PARKING_H
#define PARKING_H

#include "array.c"
#include "hash.c"
#include "queue.c"

// 停车场管理功能
// 停车场、便道和哈希表初始化 
void InitParking(Array *parkingLot,HashTable *ht,Queue *waitingQueue);
// 停车费用计算
int CalculateFee(const char *entryTime,const char *exitTime,int *hours,int *minutes,int *seconds);
// 根据车牌号码查询车辆信息
void FindCar(const HashTable *ht,const char *license); 
// 检查车牌号是否存在于停车场或便道中
bool IsCarAlreadyExists(const HashTable *ht,const Queue *waitingQueue,const char *license);
// 停车操作
void ParkCar(Array *parkingLot,HashTable *ht,Queue *waitingQueue,const char *license,const char *entryTime,int *level,int *spot); 
// 取车操作 
void LeaveCar(Array *parkingLot,HashTable *ht,Queue *waitingQueue, const char *license, const char *exitTime);
// 显示停车场所有车辆信息
void ShowParkingStatus(const Array *parkingLot);
// 显示便道所有车辆信息 
void ShowQueueStatus(const Queue *waitingQueue);

#endif 
