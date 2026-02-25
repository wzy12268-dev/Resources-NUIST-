#include <iostream>
using namespace std;

class A{
	public:
		int a;
		A(int x=0){
			a=x;
			cout<<"A::a="<<a<<endl;
		}
}; 

class B:public A{
	public:
		int a;
		B(int x,int y=5):A(x){
			a=y;
			cout<<"B::a="<<a<<endl;
		} 
}; 
int main(){
	B b(1,2);
	A *p=&b;
	cout<<"a="<<p->a<<endl;
	return 0; 
}
