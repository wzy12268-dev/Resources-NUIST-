#include "parking.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 初始化全局变量
int parkingCount=0;  // 停车场中车辆数目 
int queueCount=0;    // 便道中等候车辆数目 
 
// 停车场、便道和哈希表初始化
void InitParking(Array *parkingLot,HashTable *ht,Queue *waitingQueue){
	if(!InitArray(parkingLot,MAX_LEVELS,MAX_SPOTS)){
		printf("停车场初始化失败。\n");
		exit(EXIT_FAILURE);
	}
	CreatHashTable(ht);
	InitQueue(waitingQueue);
} 

// 停车费用计算
int CalculateFee(const char *entryTime,const char *exitTime,int *hours,int *minutes,int *seconds){
	int entryHours,entryMinutes,entrySeconds;
	int exitHours,exitMinutes,exitSeconds;
	// 解析时间 
	sscanf(entryTime,"%d:%d:%d",&entryHours,&entryMinutes,&entrySeconds);
	sscanf(exitTime,"%d:%d:%d",&exitHours,&exitMinutes,&exitSeconds);
	// 计算总秒数 
	int entryTotalSeconds=entryHours*3600+entryMinutes*60+entrySeconds;
	int exitTotalSeconds=exitHours*3600+exitMinutes*60+exitSeconds;
	
	int parkingSecond=exitTotalSeconds-entryTotalSeconds;
	if(parkingSecond<0) {
		parkingSecond+=86400;  // 处理跨天情况，补偿一天的秒数 
	}
	*hours=parkingSecond/3600;
	parkingSecond%=3600;
	*minutes=parkingSecond/60;
	*seconds=parkingSecond%60;
	
	// 停车费用计算 
	if(*hours*3600+*minutes*60+*seconds>=43200){
		return 60;  // 超过或等于12小时封顶 
	} else if (*minutes<30||*minutes==30&&*seconds==0){
		return 5*(*hours);  // 不超过30分钟不进位 
	}else if(*minutes==30&&*seconds>0||*minutes>30){
		return 5*(*hours+1);  // 超过30分钟小时数加1 
	}
} 

// 根据车牌号查询车辆信息
void FindCar(const HashTable *ht,const char *license){
	int level,spot;  // 用于存储查找到的层号和车位号 
	if(SearchHashTable(ht, license, &level, &spot)){
        printf("车牌号码：%s  层号：%d  车位号：%d\n", license, level, spot);
    } else {
        printf("未找到车牌号码为 %s 的车辆。\n", license);
    }
    printf("\n");
} 

// 检查车牌号是否存在于停车场或便道中
bool IsCarAlreadyExists(const HashTable *ht,const Queue *waitingQueue,const char *license) {
    int level, spot;
    // 检查停车场（哈希表）
    if (SearchHashTable(ht, license, &level, &spot)) {
    	printf("\n");
    	printf("该车辆已停放在停车场中。\n"); 
        return true;
    }
    // 检查便道（队列）
    int current = waitingQueue->front;
    while (current != waitingQueue->rear) {
        if (strcmp(waitingQueue->elements[current].data, license) == 0) {
        	printf("\n");
        	printf("该车辆已在便道中等候。\n");
            return true;
        }
        current = (current + 1) % QUEUE_MAX_SIZE;
    }
    return false;  // 停车场和便道中都不存在
}

// 停车操作
void ParkCar(Array *parkingLot,HashTable *ht,Queue *waitingQueue,const char *license,const char *entryTime,int *level,int *spot){
	// 如果车牌号已存在，直接返回
	if (IsCarAlreadyExists(ht, waitingQueue, license)) {
        return;  
    }
    
	Car car;
	strcpy(car.license,license);
	strcpy(car.entryTime,entryTime);
	car.flag=true;
	
	int i,j;
	for(i=0;i<MAX_LEVELS;i++){
		for(j=0;j<MAX_SPOTS;j++){
			int offset=i*parkingLot->constants[0]+j;
			if(!parkingLot->base[offset].flag){  // 找到空位 
				car.level=i+1;
				car.spot=j+1;
				// 存入二维数组 
				if(!Assign(parkingLot,i,j,&car)){
					printf("分配车位失败！\n");
					return;
				}
				parkingCount++; 
				printf("尊敬的 %s 用户，您的停车位置在停车场 %d 层 %d 号车位。\n",license,i+1,j+1);
				// 存入哈希表 
				if(!InsertIntoHashTable(ht,license,i+1,j+1)){
					printf("车辆存入哈希表失败。\n");
					return;
				}  
				// 返回层号和车位号
				if(level)
					*level=i+1;
				if(spot)
					*spot=j+1;
				return; 
			}
		}
	}
	// 若停车场已满，将车辆加入便道
	if(IsQueueFull(waitingQueue)){
		printf("尊敬的%s用户，便道已满。\n"); 
	}else{
		Enqueue(waitingQueue,license);  // 加入便道 
		queueCount++;
		printf("尊敬的%s用户，停车场已满，您已排队进入便道,序号为%d。\n",license,queueCount);
	}
}

// 车辆离开(查找需要利用哈希表)
void LeaveCar(Array *parkingLot,HashTable *ht,Queue *waitingQueue,const char *license, const char *exitTime){
	int level,spot;
	printf("\n");
	if(SearchHashTable(ht,license,&level,&spot)){
		int offset=(level-1)*parkingLot->constants[0]+(spot-1);  // 计算车辆的索引 

		// 获取车辆信息
		Car car;
		Value(parkingLot,level-1,spot-1,&car); 
		
		// 清空车位 
		parkingLot->base[offset].flag=false;  
		
		// 计算停车费用 
		int hours,minutes,seconds;
		int fee=CalculateFee(car.entryTime,exitTime,&hours,&minutes,&seconds);
		printf("尊敬的 %s 用户，您的停车时长为 %d时%d分%d秒 ，停车费为 %d 元，祝您一路顺风!\n",license,hours,minutes,seconds,fee);
		parkingCount--;
		
		// 删除哈希表中对应车辆信息
        DeleteFromHashTable(ht, license);

		// 检查车道是否为空，若不为空则进入停车，进入时间与离开汽车的离开时间相同，不考虑时间损失 
		if(!IsQueueEmpty(waitingQueue)&&parkingCount<MAX_LEVELS*MAX_SPOTS){
			char waitingLicense[STRING_MAX];
			int newlevel,newspot;
			if(Dequeue(waitingQueue,waitingLicense)){  // 从便道取出第一辆车
				ParkCar(parkingLot,ht,waitingQueue,waitingLicense,exitTime,&newlevel,&newspot);
				queueCount--; 
			}
		} 
	}else{
		printf("未找到车牌号码为 %s 的车辆。\n",license);
	}
}

// 显示停车场所有车辆信息 
void ShowParkingStatus(const Array *parkingLot){
	printf("停车场车辆信息：\n");
	int i,j;
	for(i=0;i<MAX_LEVELS;i++){
		printf("第%d层：",i+1);
		for(j=0;j<MAX_SPOTS;j++){
			Car car;
			if(Value(parkingLot,i,j,&car)){
				if(car.flag&&car.license[0]!='\0'){
					printf("[%8s] ",car.license);
				}else{
					printf("[ 未  知 ] "); 
				}
			}else{
				printf("[ 空  闲 ] "); 
			}
		}
		printf("\n");
	}
	printf("\n"); 
}

// 显示便道所有车辆信息
void ShowQueueStatus(const Queue *waitingQueue){ 
	if(IsQueueEmpty(waitingQueue)){
		printf("便道中无车辆。\n");
		printf("\n");
		return; 
	}else{
		printf("便道车辆信息：\n");
		int current=waitingQueue->front;
		int position=1;
		while(current!=waitingQueue->rear){
			printf("排序号：%d   车牌号码：%s\n",position,waitingQueue->elements[current].data);
			current=(current+1)%QUEUE_MAX_SIZE;
			position++;
		}
		printf("\n");
	}
}


