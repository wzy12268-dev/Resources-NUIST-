#ifndef ARRAY_H
#define ARRAY_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define STRING_MAX 9  // 每个车牌号最大长度 
#define MAX_DIM 2     // 最大维度 

#define MAX_LEVELS 2  // 停车场最大层数 
#define MAX_SPOTS 3   // 每层最大车位数 

// 定义单辆车的结构体
typedef struct{
	char license[STRING_MAX];  // 车牌号
	int level;                 // 层号 
	int spot;                  // 车位号 
	char entryTime[9];         // 进入时间 （格式：HH:MM:SS） 
	char exitTime[9];          // 离开时间 （格式：HH:MM:SS）
	int fee;                   // 停车费用 
	bool flag;                 // 车位是否被占用，true为占用，false为未被占用 
}Car; 

typedef struct{
	Car *base;                   // 数组基址 
	int dim;                     // 数组维度 
	int bounds[MAX_DIM];         // 每一维度的界限大小 
	int constants[MAX_DIM];      // 每一维度的映射常量，用于地址计算 
}Array; 

// 数组操作函数声明
// 初始化二维数组
int InitArray(Array *array,int rows,int cols);
// 存贮操作 
int Assign(Array *array,int i,int j,const Car *car);
// 取值操作
int Value(const Array *array,int i,int j,Car *car);
// 清空数组
void ClearArray(Array *array);
// 销毁数组
void DestroyArray(Array *array);
 
#endif 
