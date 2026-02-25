#include<iostream>
using namespace std;

int i=10;
int f1(int j){
	static int i=20;
	j=i--;
	return j;
} 
int f2(int i){
	int j=15;
	return i=j+i;
}
int main()
{
	for(int j=1;j<3;j++){
		cout<<f1(i)<<", "<<f2(i+j)<<endl;
	}
	return 0;
 } 
