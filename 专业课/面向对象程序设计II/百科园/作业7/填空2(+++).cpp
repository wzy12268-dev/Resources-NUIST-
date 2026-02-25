/*
请将如下程序补充完整，使得程序运行时的输出结果为：
B0:1
B1:2
B2:3
D:1

注意：仅允许在指定的下划线处填写代码，不得改动程序中的其他内容（需删除下划线编号）。
试题源程序：
*/
#include<iostream>
using namespace std;
class B0 {
	protected:
		int a;
	public:
		B0(int x) { a=x; } 
		void f() { cout<<"B0:"<<a<<endl; }		
};

/**********FILL**********/
//    [1]      public B0 {
class B1:virtual public B0{
		int b;
	public:		
		B1(int x, int y):B0(x),b(y) { }
		void f() { cout<<"B1:"<<b<<endl; }		
};

/**********FILL**********/
//     [2]      public B0 { 
class B2:virtual public B0{
		int b;
	public:		
		B2(int x, int y):B0(x),b(y) { }
		void f() { cout<<"B2:"<<b<<endl; }		
};

/**********FILL**********/
//class D:     [3]      {
class D:public B1,public B2{
	public:

/**********FILL**********/
//		D(int x, int y,int z):     [4]     ,B1(x,y),B2(x,z) { } 
		D(int x, int y,int z):B0(x),B1(x,y),B2(x,z) { } 
		void f() { cout<<"D:"<<a<<endl; }
};
int main() {
	D d(1,2,3);

/**********FILL**********/
	//     [5]     ;  
	d.B0::f();
	d.B1::f();
	d.B2::f();
	d.f();
	return 0;
}
