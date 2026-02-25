#include "parking.c"
 
#include <stdio.h>
#include <string.h>

int main(){
	// 定义停车场、便道、哈希表
	Array parkingLot;
	HashTable ht;
	Queue waitingQueue;
	// 初始化停车场、 便道、哈希表
	InitParking(&parkingLot,&ht,&waitingQueue);

	// 初始化的车辆
	ParkCar(&parkingLot,&ht,&waitingQueue, "苏K5665L", "09:08:20",NULL,NULL);
    ParkCar(&parkingLot,&ht,&waitingQueue, "苏AABCD1", "09:30:23",NULL,NULL);
    ParkCar(&parkingLot,&ht,&waitingQueue, "苏AABCD2", "09:47:45",NULL,NULL);
    ParkCar(&parkingLot,&ht,&waitingQueue, "苏AABCD3", "09:58:20",NULL,NULL);
	printf("\n");
	// 主菜单循环
	while(1){
		int choice;  // 用户选择操作的编号
		char license[STRING_MAX];
		char time[9];
	
		printf("欢迎使用车辆管理系统：\n"); 
		printf("    1：车辆到达\n");
		printf("    2：车辆离开\n");
		printf("    3：车辆位置信息查询\n");
		printf("    4：查看停车场所有车辆信息\n");
		printf("    5：查看便道所有车辆信息\n");
		printf("    6：查询停车场剩余停车位和便道车辆数\n");
		printf("    7：退出系统\n");
		printf("您好！请输入操作编号：");
		scanf("%d",&choice);
		printf("\n"); 
		switch(choice){
			case 1:  // 车辆到达
				printf("请输入车牌号码：");
				scanf("%s",license);
				printf("请输入车辆到达时间（HH:MM:SS）：");
				scanf("%s",time);
				ParkCar(&parkingLot,&ht,&waitingQueue,license,time,NULL,NULL);
				printf("\n");
				break; 
			
			case 2:  // 车辆离开 
				printf("请输入车牌号码：");
				scanf("%s",license);
				printf("请输入车辆离开时间（HH:MM:SS）：");
				scanf("%s",time); 
				LeaveCar(&parkingLot,&ht,&waitingQueue,license,time);
				printf("\n");
				break;
			
			case 3:  // 查看指定车辆信息 
				printf("请输入车牌号码：");
				scanf("%s",license);
				FindCar(&ht,license);
				break;
				
			case 4:  // 查看停车场所有车辆信息
				ShowParkingStatus(&parkingLot);
				break; 
			
			case 5:  // 查看便道所有车辆信息
				ShowQueueStatus(&waitingQueue);
				break;
			
			case 6:  // 查询停车场剩余停车位和便道车辆数
				printf("----正在查询停车场剩余停车位和便道车辆数----\n"); 
				printf("    停车场剩余车位数：%d\n",MAX_LEVELS*MAX_SPOTS-parkingCount);
				printf("    便道车辆数：%d\n",queueCount);
				printf("\n");
				break; 
				
			case 7:  // 退出系统
				printf("感谢使用停车场管理系统，再见！\n");
				DestroyArray(&parkingLot);
				DestroyHashTable(&ht);
				DestroyQueue(&waitingQueue);
				return 0;
				
			default:
				printf("输入无效，请重新选择。\n");
				printf("\n");
				break;
		} 	
	} 
	return 0; 
} 
