/*
请将如下程序补充完整，使得程序运行时的输出结果为：
012

注意：仅允许在指定的下划线处填写代码，不得改动程序中的其他内容（需删除下划线编号）。
试题程序：
*/
#include <iostream>  
using namespace std;  
class myClass{ 

/**********FILL**********/
	static int a; 
	public:  
		myClass() { a++; }  
		~myClass() { a--; }  
		static int getA(); 
};  
int myClass::a=0; 

/**********FILL**********/
    int myClass::getA(){ return a; } 
 
int main() {  

/**********FILL**********/
	cout<<myClass::getA(); 
	myClass d[12];  
	cout<<d[0].getA()<<endl;
	return 0;  
}
