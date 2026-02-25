#ifndef HASH_H
#define HASH_H

#include<stdbool.h>
#include "array.h"
 
#define TABLE_SIZE (MAX_LEVELS*MAX_SPOTS+1)  

// 哈希表数据结构定义 
typedef struct HashSlot{
	char license[STRING_MAX];  // 车牌号
	int level;                 // 层号
	int spot;                  // 车位号
	bool flag;                 // 是否被占用 
}HashSlot;
// 哈希表整体结构 
typedef struct HashTable{
	HashSlot *slots;  // 哈希表存储数组
	int length;       // 哈希表大小 
}HashTable;

// 哈希表操作函数声明
// 初始化哈希表 
void CreatHashTable(HashTable *ht);
// 插入车辆信息 
int InsertIntoHashTable(HashTable *ht,const char *license,int level,int spot);
// 根据车牌号查找对应信息 
int SearchHashTable(const HashTable*ht,const char *license,int *level,int *spot);
// 删除车辆信息
void DeleteFromHashTable(HashTable *ht, const char *license);
// 销毁哈希表 
void DestroyHashTable(HashTable *ht);

#endif 
