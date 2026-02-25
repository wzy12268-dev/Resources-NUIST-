/*
请将如下程序补充完整，使得程序运行时的输出结果为：
BCA
4, 2, 6
4; 2; 6

注意：仅允许在指定的下划线处填写内容，并删除下划线编号，不允许增加或删除语句，也不得改动程序中的其他部分。
试题源程序：
*/
#include <iostream>
using namespace std;
class Base1 {
	public:
		Base1(int x) { b1=x; cout<<"A"; }
		int b1;
};
class Base2 {
	public:
		Base2() { b2+=2; cout<<"B"; }

/**********FILL**********/
		static int b2; 
};
int Base2::b2=0;
class Base3 {
	public:
		Base3(int y):b3(y) { cout<<"C"; }
		int b3;
};

/**********FILL**********/
class Derived:public Base2,public Base3,public Base1 { 
	public:

/**********FILL**********/
		Derived(int x,int y):Base1(x),Base3(y){ }

/**********FILL**********/
		void Display() const{
			cout<<b1<<", "<<b2<<", "<<b3<<endl; }
};
int main() {
	const Derived d(4,6);
	cout<<endl;
	d.Display();
	cout<<d.b1<<"; "<<d.b2<<"; "<<d.b3<<endl;
	return 0;
}
