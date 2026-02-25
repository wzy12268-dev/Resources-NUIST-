#include <iostream>
using namespace std;

void fun(int &x,int &y){
	int z=x;
	x=y;
	y=z;
} 
void fun(int *x,int *y){
	int *z=x;
	x=y;
	y=z;
}
int main(){
	int x=5,y=10;
	fun(x,y);
	cout<<"x="<<x<<",y="<<y<<endl;
	fun(&x,&y);
	cout<<"x="<<x<<",y="<<y<<endl;
	return 0;
}
