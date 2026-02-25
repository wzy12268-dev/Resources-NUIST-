/*
函数findmin的功能是从数组arr（有n个元素）中找出最小元素值，并返回其引用（如果存在多个，则返回最前面的）。
请将该函数定义补充完整。
注意：仅允许在指定的下划线处填写代码，不得改动程序中的其他内容。
试题源程序如下：
*/
#include<iostream>
using namespace std;
int &findmin(int arr[], int n)
{
	int pos=0; 
	for(int i=1; i<=n-1; ++i)

/**********FILL**********/
		if(arr[i]<arr[pos]) pos= i; 

/**********FILL**********/
	return arr[pos]; 
}
int main() {
	int a[10];
	for(int i=0; i<10; i++)
		cin>>a[i];

/**********FILL**********/
	cout<<findmin(a,10)<<endl;
	return 0;
}
