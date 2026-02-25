#include <iostream>
using namespace std;

void fun(int *a,int b){
	int *k;
	k=a;
	*a=b;
	b=*k;
} 
int main(){
	int a=3,*x=&a;
	int y=5;
	fun(x,y);
	cout<<a<<' '<<y<<endl;
	return 0;
}
