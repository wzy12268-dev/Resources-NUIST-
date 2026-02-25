/*
请改正程序中指定位置的错误，使程序的输出结果如下：
x=0, y=1
x=1, y=3
x=3; y=7

注意：只允许修改注释"ERROR"的下一行，不得改动程序中的其他内容，也不允许增加或删减语句。
源程序清单：
*/
#include <iostream>  
using namespace std; 
 
/**********ERROR**********/
class Test 
{		

/**********ERROR**********/
		const int x=0;
		static int y;   
	public: 
		Test() { y+=1; }

/**********ERROR**********/
		Test(int i,int j):x(i) 
		{ y+=j; }
		void display() const;  
		void display() { cout<<"x="<<x<<", y="<<y<<endl; }  
}; 

int Test::y=0; 

/**********ERROR**********/
void Test::display() const 
{ cout<<"x="<<x<<"; y="<<y<<endl; } 

int main() {  
	Test t1; t1.display(); 
	Test t2(1,2); t2.display();
	const Test t3(3,4);  
	t3.display();  
	return 0; 
}
