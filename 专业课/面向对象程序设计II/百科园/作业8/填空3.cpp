/*
请将如下程序补充完整，使得程序运行时的输出结果为：
Base::display
Derive::display
Derive::display const

注意：仅允许在指定的下划线处填写内容，并删除下划线及其编号，不允许增加或删除语句，也不得改动程序中的其他部分。
试题源程序如下：
*/
#include <iostream>
using namespace std;
class Base {
	public:

/**********FILL**********/
		void virtual display(){
			cout<<"Base::display"<< endl; 
		}
};
class Derive:public Base { 
	public:

/**********FILL**********/
		void display() const; 
		void display() { cout<<"Derive::display"<<endl; }
};
void Derive::display() const {
	cout<<"Derive::display const"<<endl; }

/**********FILL**********/
void Display(Base &p) 
{ p.display(); } 
 
int main() {
	Base b1; Display(b1);
	Derive d1; Display(d1);
	const Derive d2; 

/**********FILL**********/
	d2.display(); 
	return 0;
}
