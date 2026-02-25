/*
请将如下程序补充完整，使得程序运行时的输出结果为：
1, 3
5
7
9

注意：仅允许在指定的下划线处填写代码，不得改动程序中的其他内容（需删除下划线）。
试题源程序：
*/
#include <iostream>  
using namespace std;  

class A { 
		int x,y; 
	public: 
 		A(int a,int b) { x=a, y=b; } 
		void show() { cout<<x<<", "<<y<<endl; } 
}; 

class B: virtual protected A {
		int k; 
  	public: 
		B(int a,int b,int c):A(a,b) { k=c; } 
		void show() { cout<<k<<endl; }  
}; 


/**********FILL**********/
class C: virtual public A{ 
		int m; 
	public: 
		C(int a,int b,int c):A(a,b) { m=c; } 
		void show() { cout<<m<<endl; } 
}; 


/**********FILL**********/
class D: public B, public C {
		int n; 
	public: 
/**********FILL**********/
		D(int a,int b,int c,int d, int e): A(a,b), B(a,b,c), C(a,b,d) { n=e; }
		void show() { cout<<n<<endl; } 
};
 
int main() { 
	D d(1,3,5,7,9); 
/**********FILL**********/
	d.A::show(); 
	d.B::show();
	d.C::show();
	d.show(); 
	return 0; 
}
