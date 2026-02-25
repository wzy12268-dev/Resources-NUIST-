/*
请改正程序中指定位置的错误，使程序的输出结果如下： 
Base::display
Derive::display
Derive::display const
Base::display
Derive::display const
Derive::display const

注意：只允许修改注释"ERROR"的下一行，不得改动程序中的其他内容，也不允许增加或删减语句。
试题源程序：
*/
#include <iostream>
using namespace std;
class Base { 
	public:

/**********ERROR**********/
		virtual void display() const{ 
			cout<<"Base::display"<< endl; 
		} 	
};
class Derive:public Base {  
	public: 

/**********ERROR**********/
		void display() const;   
		void display() { cout<<"Derive::display"<<endl; }
};
void Derive::display() const {  
	cout<<"Derive::display const"<<endl; 
}

/**********ERROR**********/
void Display(const Base &p) { 
	p.display();  
}
int main() { 
	Base b1; Derive d1;

/**********ERROR**********/
	const Derive d2; 
	b1.display(); d1.display(); d2.display(); 	 
	Display(b1); Display(d1); Display(d2); 
	return 0; 
}
