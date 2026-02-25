/*
ARRAY类用于存放一个一维int型数组，计算并输出当前数组中各元素的最大值、最小值和平均值。具体成员如下：
（1）私有成员：
l	int data[N]：数组data用于存储待处理的数据，N为常量
l	int num：当前元素的个数
l	int max：元素的最大值
l	int min：元素的最小值
l	float average：所有元素的平均值
（2）公有成员：
l	构造函数ARRAY(int x[], int n)：初始化成员数组data，n为元素个数
l	void process()：计算data数组中的最大值、最小值和平均值
l	void print()：输出当前数组中的元素及其最大值、最小值和平均值
请根据上述说明，完成ARRAY类的定义。

注意：部分源程序给出，仅允许在注释"Begin"和"End"之间填写内容，不得改动main函数和其他已有的任何内容。
试题程序：
*/
#include <iostream>
#include <fstream>
using namespace std;
const int N=100;

class ARRAY {
		int data[N];   // 数组data用于存储待处理的数据，N为常量
		int num;       // 当前元素的个数
		int max,min;   // 元素的最大值、最小值
		float average; // 元素的平均值
	public:
		ARRAY(int x[], int n); //初始化成员数组data，n为元素个数
		void process();    //计算data数组中的最大值、最小值和平均值
		void print() {    // 输出当前数组中的元素及其最大值、最小值和平均值	
			cout<<"共有"<<num<<"个元素: " ;
			for(int i=0;i<num;i++) cout<<data[i]<<' ';
			cout<<endl;
			cout<<"max="<<max<<endl;
			cout<<"min="<<min<<endl;
			cout<<"average="<<average<<endl; 
	}
};

/*******Begin*******/
ARRAY::ARRAY(int x[],int n){                     // 1
	num=n;
	for(int i=0;i<num;i++) data[i]=x[i];           
} 
void ARRAY::process(){
	max=min=data[0];
	average=0.0;                                // 记得要初始化 
	for(int i=0;i<num;i++)
	{
		if(max<data[i]) max=data[i];
		if(data[i]<min) min=data[i];
		average+=data[i];
	} 
	average/=num;
}




/*******End*********/

int main() {
	int a[N],n;
	cin>>n;  // 输入待处理元素个数 
	for(int i=0;i<n;i++) cin>>a[i];
	ARRAY arr(a,n);
	arr.process();
	arr.print();

	ifstream in1("6.1.1_3_in.dat");
	ofstream out1("6.1.1_3_out.dat");
	streambuf *cinbackup;
	streambuf *coutbackup;
	cinbackup=cin.rdbuf(in1.rdbuf());
	coutbackup=cout.rdbuf(out1.rdbuf());
	while(cin>>n) {
		for(int i=0;i<n;i++) cin>>a[i];
		ARRAY arr(a,n);
		arr.process();
		arr.print();
		cout<<endl; 
	}
	cin.rdbuf(cinbackup);
	cout.rdbuf(coutbackup);
	in1.close();
	out1.close();
	return 0;
}
