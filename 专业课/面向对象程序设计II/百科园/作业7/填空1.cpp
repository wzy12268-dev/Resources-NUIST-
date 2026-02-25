/*
请将如下程序补充完整，使得程序运行时的输出结果为：
1, 1, 2.3
2, 4.6

注意：仅允许在指定的下划线处填写内容，并删除下划线编号，不允许增加或删除语句，也不得改动程序中的其他部分。
试题源程序：
*/
#include <iostream>
using namespace std;
class Base {
	public:

/**********FILL**********/
		//Base(int a):      [1]       { } 
		Base(int a):idata(a){ } 
		void print() { cout<<idata<<", "; }
	private:
		int idata;
};

/**********FILL**********/
//class       [2]       {
class Derived:public Base{
	public:

/**********FILL**********/
		//Derived(int a, double b):      [3]      { ddata=b; }
		Derived(int a, double b):Base(a){ ddata=b; }
		void print() {

/**********FILL**********/
			//      [4]      ; 
			Base::print();
			cout<<ddata<<endl;
		}
	private:
		double ddata;
};
int main() {
	Derived d1(1,2.3), d2(2,4.6);

/**********FILL**********/
	//      [5]      ; 
	d1.Base::print();
	d1.print();
	d2.print();
	return 0;
}
