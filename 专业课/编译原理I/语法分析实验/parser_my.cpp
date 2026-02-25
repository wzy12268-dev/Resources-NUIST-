#include<bits/stdc++.h>
#include <iostream>
#include <string>
#include <fstream>
#include <set>
#include <map>
#include <iomanip>
#include <stack>
#include <cstring>   // 为 strlen
#include <cctype>    // 为 isupper
using namespace std;
const int maxnlen = 1e4;
class Grammar {
private:
	set<char>Vn;//non-terminal非终结符集合
	set<char>Vt; //terminal终结符集合
	char S;
	map<char, set<string> > P;//规则集合
	map<char,set<char> >FIRST;
	map<char,set<char> >FOLLOW;
	map<string, string>Table;
public:
	Grammar(string filename) {
		Vn.clear();
		Vt.clear();
		P.clear();
		FIRST.clear();
		FOLLOW.clear();
		ifstream in(filename.c_str());
		if (!in.is_open()) {
			cout << "文法  文件打开失败" << endl;
			exit(1);
		}
		char *buffer = new char[maxnlen];
		in.getline(buffer, maxnlen, '#');
		string temps = "";
		bool is_sethead = 0;
		for (int i = 0; i < strlen(buffer); i++) {
			if (buffer[i] == '\n' || buffer[i] == ' ')continue;
			if (buffer[i] == ';') {
				if (!is_sethead) {
					this->setHead(temps[0]);
					is_sethead = 1;
				}
				this->add(temps);
				temps = "";
			}
			else
				temps += buffer[i];
		}
		delete buffer;
		/*
			输出Vn，Vt，set
			
		*/
		
	}
	void setHead(char c) {
		S = c;
	}
	void add(string s) {
		char s1 = s[0];
		string s2="";
		int num = 0;
		for (int i = 0; i < s.length() ; i++) {
			if (s[i] == '>')num=i;
			if (num == 0)continue;
			if (i > num)
				s2 += s[i];
		}
		s2 += ';';
		Vn.insert(s1);
		string temp = "";
		//set<char>::iterator iter1 = s2.begin();
		for (int i = 0; i < s2.length() ; i++) {//char s : s2
			char s=s2[i];
			if (!isupper(s) && s != '|'&&s != ';'&&s!='@')Vt.insert(s);
			if (s == '|' || s == ';') {
				P[s1].insert(temp);
				temp = "";
			}
			else {
				temp += s;
			}
		}
	}
	void print() {
		cout << "当前分析文法为：" << endl << endl;
		for (set<char>::iterator it = Vn.begin(); it != Vn.end(); it++) {
			char cur_s = *it;
			for (set<string>::iterator it1 = P[cur_s].begin(); it1 != P[cur_s].end(); it1++) {
				string cur_string = *it1;
				cout << cur_s << "->" << cur_string << endl;
			}
		}
	}
	void getFirst() {
    	FIRST.clear();
    	int iter = 4;

    	while (iter--) {
        	for (set<char>::iterator it = Vn.begin(); it != Vn.end(); ++it) {
            	char cur_s = *it;  // ? cur_s 在这里定义

            	for (set<string>::iterator it1 = P[cur_s].begin(); it1 != P[cur_s].end(); ++it1) {
                	string cur_string = *it1;
                	if (cur_string.empty()) continue;

                	char X = cur_string[0];

                	// 情况1：终结符或@
                	if (!isupper(X)) {
                    	FIRST[cur_s].insert(X);
                	}
                	// 情况2：非终结符
                	else {
                    	for (auto ch : FIRST[X]) {
                      	  FIRST[cur_s].insert(ch);
                    	}
                	}
            	}
        	}
    	}

    	cout << "FIRST集为：" << endl << endl;
    	for (set<char>::iterator it = Vn.begin(); it != Vn.end(); ++it) {
        	char cur_s = *it;
        	cout << "FIRST()   " << cur_s;
        	for (set<char>::iterator it1 = FIRST[cur_s].begin(); it1 != FIRST[cur_s].end(); ++it1) {
            	cout << "       " << *it1;
        	}
        	cout << endl;
    	}
	}

	void getFollow() {
		FOLLOW.clear();
		FOLLOW[S].insert('#');
		//判断迭代次数
		int iter = 4;
		while (iter--) {
			for (set<char>::iterator it = Vn.begin(); it != Vn.end(); it++) {
				char cur_s = *it;
				/*请编程实现以下功能
				***************************************************************************************
				cur_s->cur_string[0]
				a加到A的FIRST集
				cur_s->cur_string[0]
				B的FITRST集加到A的FIRST集
				*/
				for (set<string>::iterator it1 = P[cur_s].begin(); it1 != P[cur_s].end(); it1++) {
					string cur_string = *it1;
					for (int i = 0; i < (int)cur_string.length(); i++) {
						char A1 = cur_string[i];
						if (!isupper(A1)) continue;          // 只处理非终结符 A

						bool needFollowLeft = true;          // 是否需要把 FOLLOW(cur_s) 加入 FOLLOW(A)
						int j = i + 1;

						// 从 A 后面开始往后看：β = X1 X2 ...
						while (j < (int)cur_string.length()) {
    						char X = cur_string[j];

    						// Step1：A 后面紧跟终结符，直接加入 FOLLOW(A)
    						if (!isupper(X)) {
        						FOLLOW[A1].insert(X);
        						needFollowLeft = false;
        						break;
    						}

    						// Step2 + Step3：A 后面是非终结符 C，把 FIRST(C)\{ @ } 加入 FOLLOW(A)
    						for (auto ch : FIRST[X]) {
        						if (ch != '@') FOLLOW[A1].insert(ch);
    						}

    						// 如果 FIRST(C) 含 @，继续看下一个符号（对应 AC / ACK / ACKM... 的情况）
    						if (FIRST[X].count('@')) {
        						j++;
    						} else {
        						needFollowLeft = false;
        						break;
    						}
						}

						// Step4：如果 A 后面的串都能推出空（或 A 已在末尾），把 FOLLOW(左部 cur_s) 加入 FOLLOW(A)
						if (needFollowLeft) {
    						for (auto ch : FOLLOW[cur_s]) {
        						FOLLOW[A1].insert(ch);
    						}
						}
					}
				}
			}
		}
		//输出FOLLOW集
		cout << "FOLLOW集为：" << endl << endl;
		for (set<char>::iterator it = Vn.begin(); it != Vn.end(); it++) {
			char cur_s = *it;
			cout << "FOLLOW()  " << cur_s;
			for (set<char>::iterator it1 = FOLLOW[cur_s].begin(); it1 != FOLLOW[cur_s].end(); it1++) {
				cout << "       " << *it1;
			}
			cout << endl;
		}
	}
	void getTable() {
		set<char>Vt_temp;
		//int i = 0; i < s2.length() ; i++
		//set<char>::iterator iter1;
		for (set<char>::iterator iter1 =Vt.begin(); iter1!=Vt.end();iter1++ ) {
			//char c=Vt[iter1];
			Vt_temp.insert(*iter1);
		}
		Vt_temp.insert('#');
		for (auto it = Vn.begin(); it != Vn.end(); it++) {
			char cur_s = *it;
			for (auto it1 = P[cur_s].begin(); it1 != P[cur_s].end(); it1++) {
				/*
				产生式为
					cur_s->cur_string
				*/
				string cur_string = *it1;
				if (isupper(cur_string[0])) {
					char first_s = cur_string[0];
					for (auto it2 = FIRST[first_s].begin(); it2 != FIRST[first_s].end(); it2++) {
						string TableLeft = "";
						TableLeft = TableLeft +cur_s + *it2;
						Table[TableLeft] = cur_string;
					}
					
				}
				else {
					string TableLeft = "";
					TableLeft = TableLeft+ cur_s + cur_string[0];
					Table[TableLeft] = cur_string;
				}	
			}
			if (FIRST[cur_s].count('@') > 0) {
				for (auto it1 = FOLLOW[cur_s].begin(); it1 != FOLLOW[cur_s].end(); it1++) {
					string TableLeft = "";
					TableLeft =TableLeft+ cur_s + *it1;
					Table[TableLeft] = "@";
				}
			}
		}
		/*
			检查出错信息：即表格中没有出现过的
		*/
	
		for (auto it = Vn.begin(); it != Vn.end(); it++) {
			for (auto it1 = Vt_temp.begin(); it1 != Vt_temp.end(); it1++) {
				string TableLeft = "";
				TableLeft =TableLeft+ *it + *it1;
				if (!Table.count(TableLeft)) {
					Table[TableLeft] = "error";
				}
			}
		}
		
		/*请编程实现以下功能
		***************************************************************************************				
			显示Table，例如格式打印：cout << *it << "->" << setw(7) << Table[iter];
		*/
		cout << "显示table表：" << endl << endl;
		cout << setw(8) << " ";
		for (auto it = Vt_temp.begin(); it != Vt_temp.end(); it++) {
    		cout << setw(8) << *it;
		}
		cout << endl;

		for (auto it = Vn.begin(); it != Vn.end(); it++) {
    		cout << setw(8) << *it;
    		for (auto it1 = Vt_temp.begin(); it1 != Vt_temp.end(); it1++) {
        		string key = "";
        		key = key + *it + *it1;
        		cout << setw(8) << Table[key];
    		}
    		cout << endl;
		}
	}
	/*
		每一次分析一个输入串
		Sign为符号栈,出栈字符为x
		输入字符串当前字符为a
	*/
	bool AnalyzePredict(string inputstring){
    	// 自动补结束符
    	if (inputstring.empty() || inputstring.back() != '#') inputstring += "#";

    	stack<char> Sign;
    	Sign.push('#');
    	Sign.push(S);

    	int StringPtr = 0;
    	char a = inputstring[StringPtr];   // 注意：这里不先 ++

    	while (!Sign.empty()) {
        	char x = Sign.top();

        	// 1) 终结符 或 #
        	if (Vt.count(x) || x == '#') {
    	        if (x != a) return false;          // 不匹配 -> reject
     	       Sign.pop();                        // 匹配 -> 弹栈

     	       if (x == '#') return true;         // 匹配到 # -> accept

      	      // 前移输入指针
    	        StringPtr++;
    	        if (StringPtr >= (int)inputstring.size()) return false; // 防越界
    	        a = inputstring[StringPtr];
    	    }
     		   // 2) 非终结符：查表并展开
        	else {
        	    string key = "";
        	    key = key + x + a;

        	    if (!Table.count(key) || Table[key] == "error") return false;

        	    string prod = Table[key];
        	    Sign.pop(); // 弹出非终结符 x

        	    if (prod != "@") {
        	        for (int i = (int)prod.size() - 1; i >= 0; --i) {
        	            Sign.push(prod[i]);
        	        }
        	    }
        	}
    	}
    	return false;
	}

	/*
		消除左递归
	*/
	void remove_left_recursion(){
		string tempVn = "";
		for (auto it = Vn.begin(); it != Vn.end(); it++) {
			tempVn += *it;
		}
		
		for (int i = 0; i < tempVn.length(); i++) {
			char pi = tempVn[i];
			/*请编程实现消除左递归的功能
				***************************************************************************************
			*/
			for (int j = 0; j < i; j++) {
    			char pj = tempVn[j];
    			set<string> newSet;

    			for (auto right : P[pi]) {
        			if (right[0] == pj) {
            			for (auto sub : P[pj]) {
                			string prefix = (sub == "@") ? "" : sub;
							string newRight = prefix + right.substr(1);
							if (newRight.empty()) newRight = "@";
                			newSet.insert(newRight);
            			}
        			} else {
            			newSet.insert(right);
        			}
    			}
    			P[pi] = newSet;
			}
			remove_left_gene(pi);
		}
	}
	/*
		提取左因子
	*/
	void remove_left_gene(char c) {
		char NewVn;
		for (int i = 0; i < 26; i++) {
			NewVn = i + 'A';
			if (!Vn.count(NewVn)) {
				break;
			}
		}
		bool isaddNewVn = 0;
		for (auto it = P[c].begin(); it != P[c].end(); it++) {
			string right = *it;
			
			if (right[0] == c) {
				isaddNewVn = 1;
				
				break;
			}
		}
		if (isaddNewVn) {
			set<string>NewPRight;
			set<string>NewPNewVn;
			for (auto it = P[c].begin(); it != P[c].end(); it++) {
				string right = *it;
				if (right[0] != c) {
					right += NewVn;
					NewPRight.insert(right);
				}
				else {
					right = right.substr(1);
					right += NewVn;
					NewPNewVn.insert(right);
				}
			}
			Vn.insert(NewVn);
			NewPNewVn.insert("@");
			P[NewVn] = NewPNewVn;
			P[c] = NewPRight;
		}
	}
	void ShowByTogether() {
		for (auto it = Vn.begin(); it != Vn.end(); it++) {
			cout << *it << "->";
			char c = *it;
			for (auto it1 = P[c].begin(); it1 != P[c].end(); it1++) {
				if (it1 == P[c].begin())cout << *it1;
				else
					cout << "|" << *it1;
					
			}
			cout << endl;
		}
	}
};
int main() {
	/*
	文法测试
	E->T|E+T;
	T->F|T*F;
	F->i|(E);

	A->+TA|@;
	B->*FB|@;
	E->TA;
	F->(E)|i;
	T->FB;
	直接将上面两个测试样例放在parse_test1.txt和parse_test2.txt中
	*/
	string filename_gramer = "D:\\桌面\\实验二-语法分析实验课-凌Dev-C\\parse_test3.txt"; 
	Grammar *grammar=new Grammar(filename_gramer);
	grammar->setHead('E');  
	cout << "/-------------------------没有消除左递归-----------------------------/" << endl;
	cout << "规格显示：" << endl;
	grammar->ShowByTogether();
	cout << endl;
	grammar->getFirst();
	cout << endl;
	grammar->getFollow();
	cout << endl;
	grammar->getTable();
	
	cout << "/--------------------------------------------------------------------/" << endl<<endl<<endl;


	cout << "/-------------------------已经消除左递归-----------------------------/" << endl;
	/*
	消除左递归之后的判断
	*/
	grammar->remove_left_recursion();
	cout << "规格显示：" << endl;
	cout << endl;
	grammar->ShowByTogether();
	grammar->getFirst();
	cout << endl;
	grammar->getFollow();
	cout << endl;
	grammar->getTable();
	
	cout << "\n================= LL(1) 识别测试（用 i 表示标识符/数字） =================\n";
	cout << "请输入要识别的符号串（仅包含 i,+,-,*,/,(,) ，例如 i+i-(i*i)/i ）\n";
	cout << "输入 q 退出。\n\n";

	while (true) {
    	cout << "Input> ";
    	string s;
    	cin >> s;
    	if (s == "q" || s == "Q") break;

    	bool ok = grammar->AnalyzePredict(s);
    	if (ok) cout << "结果：该串是当前文法的句子（ACCEPT）\n";
    	else    cout << "结果：该串不是当前文法的句子（REJECT）\n";
    	cout << endl;
	} 

	cout << "/--------------------------------------------------------------------end/" << endl << endl << endl;
	system("pause");
	return 0;
}

