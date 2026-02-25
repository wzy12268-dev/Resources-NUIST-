#include <iostream>
using namespace std;

class Part{
	public:
		Part(int x=0):val(x){
			cout<<val;
		}
		~Part(){
			cout<<val;
		}
	private:
		int val;
};
class Whole{
	public:
		Whole(int x,int y,int z=0):p2(x),p1(y),val(z){
			cout<<val;
		}
		~Whole(){
			cout<<val;
		}
	private:
		Part p1,p2;
		int val;
};
int main(){
	Whole obj(1,2,3);
	return 0;
}
