/*
学生（Stu）主要用于处理某个学生的成绩，主要成员如下：
（1）私有成员：
long number   // 学生学号 
string name    // 学生姓名 
int n          // 课程数量
float *score   // 用于指向保存学生成绩的数组
float average  // 所有课程成绩的平均分 
int count      // 不及格（<60)的课程数量
（2）公有成员：
Stu(int n)  // 构造函数，动态创建一个长度为n的数组，返回值赋给score，并将count，average初始化为0
~Stu()      // 析构函数，删除成绩数组 
void input()      // 依次输入学生的学号、姓名以及n门课程的成绩 
void process()  // 统计学生所有课程的平均分，以及不及格的课程数量 
void print()      // 输出学生的学号、姓名、平均分以及不及格课程数
请根据上述说明，完成Stu类的定义。

注意：部分源程序给出，仅允许在注释"Begin"和"End"之间填写内容，不得改动main函数和其他已有的任何内容。
试题程序：
*/
#include<iostream>
#include<fstream>
#include<string>
using namespace std;

class Stu {
		long number;   // 学生学号 
		string name;   // 学生姓名 
		int n;         // 课程数量
		float *score;  // 用于指向保存学生成绩的数组
		float average; // 所有课程成绩的平均分 
		int count;     // 不及格（<60)的课程数量
	public:
		Stu(int n); // 构造函数，动态创建一个长度为n的数组，返回值赋给score，并将count，average初始化为0
		~Stu();     // 析构函数，删除成绩数组 
		void input();    // 依次输入学生的学号、姓名以及n门课程的成绩 
		void process();  // 统计学生所有课程的平均分，以及不及格的课程数量
		void print() {   // 输出学生的学号、姓名、平均分以及不及格课程数
			cout<<"学号："<<number<<"   姓名："<<name<<endl;
			cout<<"平均分："<<average<<" 不及格数："<<count<<endl;
		}  
};  

/*******Begin*******/
Stu::Stu(int n){
	this->n=n;
	score=new float[n];
	count=0;
	average=0;
} 
Stu::~Stu(){delete[] score;}
void Stu::input(){
	cin>>number>>name;
	for(int i=0;i<n;i++)
	{
		cin>>score[i];
	}
} 
void Stu::process(){
	for(int i=0;i<n;i++)
	{
	
	if(score[i]<60) count++;
	average+=score[i];
	}
	average/=n;
}



/*******End*********/

int main() {
	int n;
	cin>>n;
	Stu s(n);
	s.input();
	s.process();
	s.print();

	ifstream in1("6.6.1_2_in.dat");
	ofstream out1("6.6.1_2_out.dat");
	streambuf *cinbackup;
	streambuf *coutbackup;
	cinbackup=cin.rdbuf(in1.rdbuf());
	coutbackup=cout.rdbuf(out1.rdbuf());	
	while(cin>>n) {
		Stu s(n);
		s.input();
		s.process();
		s.print();
	}	
	cin.rdbuf(cinbackup);
	cout.rdbuf(coutbackup);
	in1.close();
	out1.close();
	return 0;
}
