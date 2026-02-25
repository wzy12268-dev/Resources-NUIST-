#include<iostream>
using namespace std;

class B1{
	int b1;
	public:
		B1(int i=0){
			b1=i;
			cout<<b1;
		}
}; 

class B2{
	int b2;
	public:
		B2(int i){
			b2=i;
			cout<<b2; 
		}
};

class B3{
	int b3;
	public:
		B3(int i){
			b3=i;
			cout<<b3<<endl; 
		}
};

class D{
	int d;
	B2 b2;
	B1 b1;
	B3 b3;
	public:
		D(int i,int j,int k):b2(i),b3(j){
			d=k;
			cout<<"D::"<<k<<endl;
		}
};

int main(){
	D d(1,2,3);
	return 0;
}
