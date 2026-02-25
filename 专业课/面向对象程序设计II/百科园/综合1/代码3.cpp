#include <iostream>
using namespace std;

int a=4;
int main(){
	int b=7,c=10;
	cout<<a<<", "<<b<<", "<<c<<endl;
	{
		int b=6;
		float c=8.8;
		cout<<a<<", "<<b<<", "<<c<<endl;
		a=b;
	}
	cout<<a<<", "<<b<<", "<<c<<endl;
	return 0; 
} 
