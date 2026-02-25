/*
请改正程序中指定位置的错误，使程序的输出结果如下： 
x=10, Previous=9, Next=11
注意：只允许修改注释"ERROR"的下一行，不得改动程序中的其他内容，也不允许增加或删减语句。
源程序清单：
*/
#include<iostream>
using namespace std;

/**********ERROR**********/
//void prevnext(int x,int prev,int next)  {
void prevnext(int x,int &prev,int &next)  {  // 注意x前不要加&号 

/**********ERROR**********/
	//prev=x--;  
	prev=x-1; 
	next=++x; 
}
int main() {
	int x=10,y,z;
	prevnext(x,y,z);
	cout<<"x="<<x<<", Previous="<<y<<", Next="<<z<<endl;
	return 0;
}
