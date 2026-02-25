#include "array.h"

// 初始化二维数组
int InitArray(Array *array,int rows,int cols){    
	// rows为行数，cols为列数
	array->dim=MAX_DIM;            // 初始化为二维数组
	array->bounds[0]=rows;         // 第一维大小
	array->bounds[1]=cols;         // 第二维大小
	array->base=(Car *)malloc(rows*cols*sizeof(Car));  // 顺序存储
	if(!array->base){
		printf("内存分配失败\n");
		return 0;
	}  
	int i;
	for(i=0;i<rows*cols;i++){
		array->base[i].flag=false;
	}
	array->constants[1]=1;     // 第二维映射常量
	array->constants[0]=cols;  // 第一维映射常量（行序）
	return 1;  // 初始化成功 
}

// 存贮操作 
int Assign(Array *array,int i,int j,const Car *car){
	if(i<0||i>=array->bounds[0]||j<0||j>=array->bounds[1]){
		printf("越界访问。\n");
		return 0;
	}
	int offset=i*array->constants[0]+j*array->constants[1];
	array->base[offset]=*car;
	array->base[offset].flag=true;
	return 1;
} 

// 取值操作
int Value(const Array *array,int i,int j,Car *car){
	if(i<0||i>=array->bounds[0]||j<0||j>=array->bounds[1]){
		printf("越界\n");
		return 0;
	}
	int offset=i*array->constants[0]+j*array->constants[1];
	if(!array->base[offset].flag){
		return 0;
	}
	*car=array->base[offset];
	return 1;
} 

// 清空数组
void ClearArray(Array *array){
	int i;
	if(!array->base) return;  // 若数组未初始化，直接返回
	int totalElements=array->bounds[0]*array->bounds[1];  // 元素总个数
	for(i=0;i<totalElements;i++){
		array->base[i].flag=false;
	} 
} 

// 销毁数组
void DestroyArray(Array *array){
	if(array->base){
	 	free(array->base);  // 释放内存 
		array->base=NULL;
	}
} 


