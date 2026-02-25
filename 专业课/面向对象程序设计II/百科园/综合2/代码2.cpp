#include<iostream>
using namespace std;
class A{
	public:
		int a;
		A(int x=0){a=x;
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
	B b(3);
	A &r=b;
	cout<<"a="<<r.a<<endl;
	return 0; 
}
