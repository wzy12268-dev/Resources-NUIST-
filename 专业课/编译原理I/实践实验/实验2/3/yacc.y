%{
/*本yacc的生成文件是yacc.tab.c和yacc.tab.h
yacc文件由3段组成，用2个%%行把这3段隔开。
第1段是声明段，包括：
1-C代码部分：include头文件、函数、类型等声明，这些声明会原样拷到生成的.c文件中。
2-记号声明，如%token
3-类型声明，如%type
第2段是规则段，是yacc文件的主体，包括每个产生式是如何匹配的，以及匹配后要执行的C代码动作。
第3段是C函数定义段，如yyerror()的定义，这些C代码会原样拷到生成的.c文件中。该段内容可以为空*/

//第1段：声明段
#include "main.h"//lex和yacc要共用的头文件，里面包含了一些头文件，重定义了YYSTYPE

extern "C"//为了能够在C++程序里面调用C函数，必须把每一个需要使用的C函数，其声明都包括在extern "C"{}块里面，这样C++链接时才能成功链接它们。extern "C"用来在C++环境下设置C链接类型。
{    //lex.l中也有类似的这段extern "C"，可以把它们合并成一段，放到共同的头文件main.h中
    void yyerror(const char *s);
    extern int yylex(void);//该函数是在lex.yy.c里定义的，yyparse()里要调用该函数，为了能编译和链接，必须用extern加以声明
}

%}

/*lex里要return的记号的声明
用token后加一对<member>来定义记号，旨在用于简化书写方式。
假定某个产生式中第1个终结符是记号OPERATOR，则引用OPERATOR属性的方式：
1-如果记号OPERATOR是以普通方式定义的，如%token OPERATOR，则在动作中要写$1.m_cOp，以指明使用YYSTYPE的哪个成员
2-用%token<m_cOp>OPERATOR方式定义后，只需要写$1，yacc会自动替换为$1.m_cOp
另外用<>定义记号后，非终结符如file, tokenlist，必须用%type<member>来定义(否则会报错)，以指明它们的属性对应YYSTYPE中哪个成员，这时对该非终结符的引用，如$$，会自动替换为$$.member*/
%token<m_nInt>INTEGER
%token<m_sId>IDENTIFIER
%token<m_cOp>OPERATOR
%type<m_sId>file
%type<m_sId>tokenlist

%%

file:    //文件，由记号流组成
    tokenlist    //这里仅显示记号流中的ID
    {
        cout<<"all id:"<<$1<<endl;    //$1是非终结符tokenlist的属性，由于该终结符是用%type<m_sId>定义的，即约定对其用YYSTYPE的m_sId属性，$1相当于$1.m_sId，其值已经在下层产生式中赋值(tokenlist IDENTIFIER)
    };
tokenlist://记号流，或者为空，或者由若干数字、标识符、及其它符号组成
    {
    }
    | tokenlist INTEGER
    {
        cout<<"int: "<<$2<<endl;//$2是记号INTEGER的属性，由于该记号是用%token<m_nInt>定义的，即约定对其用YYSTYPE的m_nInt属性，$2会被替换为yylval.m_nInt，已在lex里赋值
    }
    | tokenlist IDENTIFIER
    {
        $$+=" " + $2;//$$是非终结符tokenlist的属性，由于该终结符是用%type<m_sId>定义的，即约定对其用YYSTYPE的m_sId属性，$$相当于$$.m_sId，这里把识别到的标识符串保存在tokenlist属性中，到上层产生式里可以拿出为用
        cout<<"id: "<<$2<<endl;//$2是记号IDENTIFIER的属性，由于该记号是用%token<m_sId>定义的，即约定对其用YYSTYPE的m_sId属性，$2会被替换为yylval.m_sId，已在lex里赋值
    }
    | tokenlist OPERATOR
    {
        cout<<"op: "<<$2<<endl;//$2是记号OPERATOR的属性，由于该记号是用%token<m_cOp>定义的，即约定对其用YYSTYPE的m_cOp属性，$2会被替换为yylval.m_cOp，已在lex里赋值
    };

%%

void yyerror(const char *s)    //当yacc遇到语法错误时，会回调yyerror函数，并且把错误信息放在参数s中
{
    cerr<<s<<endl;//直接输出错误信息
}

int main()//程序主函数，这个函数也可以放到其它.c, .cpp文件里
{
    const char* sFile="file.txt";//打开要读取的文本文件
    FILE* fp=fopen(sFile, "r");
    if(fp==NULL)
    {
        printf("cannot open %s\n", sFile);
        return -1;
    }
    extern FILE* yyin;    //yyin和yyout都是FILE*类型
    yyin=fp;//yacc会从yyin读取输入，yyin默认是标准输入，这里改为磁盘文件。yacc默认向yyout输出，可修改yyout改变输出目的

    printf("-----begin parsing %s\n", sFile);
    yyparse();//使yacc开始读取输入和解析，它会调用lex的yylex()读取记号
    puts("-----end parsing");

    fclose(fp);

    return 0;
}