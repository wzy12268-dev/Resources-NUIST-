#include "hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 哈希函数(将车牌号字符转换为哈希值--整数索引)
int HashFunction(const char *license,int length){
	unsigned int hash=0;
	const char *start=license+1;  // 从第二个字符开始计算,因为第一个字符为汉字 
	while(*start){
		hash=(hash*31+*start)%length;
		start++;
	}
	return hash;
} 

// 初始化哈希表
void CreatHashTable(HashTable *ht){
	ht->length=TABLE_SIZE;  // 设置哈希表大小 
	ht->slots=(HashSlot *)malloc(ht->length*sizeof(HashSlot));
	if(!ht->slots){
		printf("内存分配失败。\n");
		exit(EXIT_FAILURE);
	}
	int i;
	for(i=0;i<ht->length;i++){
		ht->slots[i].flag=false;  // 初始化每个槽位为未占用 
		ht->slots[i].license[0]='\0';
	} 
} 

// 插入车辆信息
int InsertIntoHashTable(HashTable *ht,const char *license,int level,int spot){
	int index=HashFunction(license,ht->length);  // 计算哈希索引
	int originalIndex=index; 
	// 线性探测解决冲突 
	while(ht->slots[index].flag){
		index=(index+1)%ht->length;
		if(index==originalIndex){
			printf("哈希表已满，无法插入车牌号。\n");
			return 0;
		} 
	} 
	strcpy(ht->slots[index].license,license);
	ht->slots[index].level=level;
	ht->slots[index].spot=spot;
	ht->slots[index].flag=true;

	return 1;
} 

// 根据车牌号查找对应信息
int SearchHashTable(const HashTable *ht,const char *license,int *level,int *spot){
	int index=HashFunction(license,ht->length);  // 计算哈希索引 
	int originalIndex=index;
	
	while(ht->slots[index].flag){

		if(strcmp(ht->slots[index].license,license)==0){
			*level=ht->slots[index].level;
			*spot=ht->slots[index].spot;
			return 1; 
		}
		index=(index+1)%ht->length;
		if(index==originalIndex){
			break;  // 已循环一圈 
		}
	}
	return 0; 
} 

// 删除车辆信息
void DeleteFromHashTable(HashTable *ht, const char *license) {
    int index = HashFunction(license, ht->length);
    int originalIndex = index;
    
    while (ht->slots[index].flag) {
        if (strcmp(ht->slots[index].license, license) == 0) {
            ht->slots[index].flag = false;   // 标记该槽位为空
            ht->slots[index].license[0] = '\0';
            return;
        }
        index = (index + 1) % ht->length;
        if (index == originalIndex) {
            break;  // 已循环一圈
        }
    }
}

// 销毁哈希表
void DestroyHashTable(HashTable *ht){
	free(ht->slots);
	ht->slots=NULL;
	ht->length=0;
} 
