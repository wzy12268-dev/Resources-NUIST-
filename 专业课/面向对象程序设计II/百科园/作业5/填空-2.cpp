/*
请将如下程序补充完整，使得程序运行时的输出结果为：
x=0, y=1
x=1, y=3
x=3; y=7

注意：仅允许在指定的下划线处填写内容，并删除下划线编号，不允许增加或删除语句，也不得改动程序中的其他部分。
试题源程序：
*/
#include <iostream>  
using namespace std;  
class Test {

/**********FILL**********/
		const int x=0;

/**********FILL**********/
		static int y; 
	public: 
		Test() { y+=1; }
		Test(int i,int j):x(i) { y+=j; } 
		void display() const;  

/**********FILL**********/
		void diaplay(){ cout<<"x="<<x<<", y="<<y<<endl; } 
}; 
int Test::y=0;  

/**********FILL**********/
void Test::display() const {
	cout<<"x="<<x<<"; y="<<y<<endl;
} 
int main() {  
	Test t1; t1.display(); 
	Test t2(1,2); t2.display();
	const Test t3(3,4);  
	t3.display(); 
	return 0; 
}
