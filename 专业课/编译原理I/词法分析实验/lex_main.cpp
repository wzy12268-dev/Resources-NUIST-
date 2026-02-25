#include <bits/stdc++.h>
using namespace std;

/* =================== 配置与全局缓冲 =================== */

static const int MAX_BUF = 200000;      // 源文件大小上限
static const int MAX_TOKEN = 1024;
static const int MAX_ID   = 4096;

FILE *fp = nullptr;     // 输入 testHard.c
FILE *fp1 = nullptr;    // 输出 lex_out.txt

// 源与过滤后文本
static char resourceProject[MAX_BUF];   // 读入后 + 过滤后覆盖
static int  pProject = 0;               // 过滤后串的扫描指针

// 过滤后每个字符 -> 原文行列
static int dstLineOf[MAX_BUF];
static int dstColOf [MAX_BUF];

// 被删除字符的原文行列记录
static int removedLine[MAX_BUF];
static int removedCol [MAX_BUF];
static int removedCnt = 0;

// 在过滤阶段发现 “块注释未闭合”
static bool unterminatedCommentFound = false;

// 词法单元当前属性串
static char tokenLexeme[MAX_TOKEN];

// 标识符表（只存标识符名）
static char IdentifierTbl[MAX_ID][128];

// C 语言 32 个关键字（小写）
static const char *reserveWord[32] = {
    "auto","break","case","char","const","continue","default","do",
    "double","else","enum","extern","float","for","goto","if",
    "int","long","register","return","short","signed","sizeof","static",
    "struct","switch","typedef","union","unsigned","void","volatile","while"
};

// 常见运算符/界符（用于打印时对齐原有编码 33..）
// 注意：复合赋值另外用专用分支，不纳入此表
static const char *operatorOrDelimiter[] = {
    "+","-","*","/","%","++","--",           // 33..39
    "=", "==","!=",                          // 40..42
    ">","<",">=","<=",                       // 43..46
    "&&","||","!",                           // 47..49
    "&","|","^","~",                         // 50..53
    "<<",">>",                               // 54..55
    "(",")","[","]","{","}",                 // 56..61
    ",",";",".",":","?","#"                  // 62..68 （可按需增减）
};
static const int OPDEL_BASE = 33;
static const int OPDEL_CNT  = sizeof(operatorOrDelimiter)/sizeof(operatorOrDelimiter[0]);

/* ================ token code（可根据需要扩展） =================
   0   : EOF
   1..32  : 关键字
   33..(33+OPDEL_CNT-1) : 上表中的运算符/界符
   70 : <<=
   71 : >>=
   72 : +=
   73 : -=
   74 : *=
   75 : /=
   76 : %=
   77 : &=
   78 : |=
   79 : ^=
   99 : 十进制整数
   100: 标识符
   101: 浮点数（十进制）
   103: 八进制整数
   104: 十六进制整数
   105: 二进制整数
   -2 : 词法错误（非法字符或格式错误）
   =============================================================== */

static inline bool IsLetter(char c) {
    return (c=='_' || (c>='a' && c<='z') || (c>='A' && c<='Z'));
}
static inline bool IsDigit(char c) {
    return (c>='0' && c<='9');
}
static inline bool IsHex(char c) {
    return ( (c>='0'&&c<='9') || (c>='a'&&c<='f') || (c>='A'&&c<='F') );
}

int searchReserve(const char *lex) {
    for (int i=0;i<32;i++) if (strcmp(reserveWord[i], lex)==0) return i+1;
    return -1;
}

/* ================ 过滤注释 + 删除空白（保留空格和 \n） ================= */
void filterResource(char r[]) {
    char out[MAX_BUF];
    int  o = 0;
    int  line = 1, col = 1;
    removedCnt = 0;
    unterminatedCommentFound = false;

    auto record_removed = [&](int li, int co) {
        if (removedCnt < MAX_BUF) {
            removedLine[removedCnt] = li;
            removedCol [removedCnt] = co;
            removedCnt++;
        }
    };

    for (int i=0; r[i] != '\0'; ) {
        char c = r[i];

        // 行注释 //
        if (c=='/' && r[i+1]=='/') {
            int li=line, co=col;
            record_removed(li,co); i++; col++;
            record_removed(li,co+1); i++; col++;
            while (r[i] && r[i] != '\n') { record_removed(line,col); i++; col++; }
            if (r[i]=='\n') {
                // 保留换行
                out[o]='\n'; dstLineOf[o]=line; dstColOf[o]=col; o++;
                i++; line++; col=1;
            }
            continue;
        }
        // 块注释 /* ... */
        if (c=='/' && r[i+1]=='*') {
            record_removed(line,col); i++; col++;
            record_removed(line,col); i++; col++;
            bool closed=false;
            while (r[i]) {
                if (r[i]=='*' && r[i+1]=='/') {
                    record_removed(line,col); i++; col++;
                    record_removed(line,col); i++; col++;
                    closed=true; break;
                }
                if (r[i]=='\n') {
                    record_removed(line,col); i++; line++; col=1;
                } else {
                    record_removed(line,col); i++; col++;
                }
            }
            if (!closed) {
                unterminatedCommentFound = true;
            }
            continue;
        }

        // 删除除空格以外的空白（\t \r \v \f），保留空格与换行
        if (c=='\t' || c=='\r' || c=='\v' || c=='\f') {
            record_removed(line,col); i++; col++;
            continue;
        }

        // 正常拷贝（含空格和换行）
        out[o]=c; dstLineOf[o]=line; dstColOf[o]=col; o++;
        if (c=='\n') { line++; col=1; } else { col++; }
        i++;
    }

    out[o]='\0';
    strcpy(r, out);
}

/* ==================== 字符/字符串 扫描（可报未闭合错） ==================== */

struct ScanResult { int syn; int len; }; // syn=-2 表示词法错误

ScanResult scanStringLiteral(const char *s) {
    // s[0] == '"'
    int i=1; bool esc=false;
    while (s[i]) {
        char c = s[i];
        if (!esc) {
            if (c=='\\') { esc=true; i++; continue; }
            if (c=='"')  { return { /*102=字符串*/ 102, i+1 }; }
            if (c=='\n') { return { -2, i }; } // 未闭合
            i++;
        } else {
            esc=false; i++;
        }
    }
    return { -2, i }; // 到 EOF 仍未闭合
}

ScanResult scanCharLiteral(const char *s) {
    // s[0] == '\''
    int i=1; bool esc=false;
    while (s[i]) {
        char c = s[i];
        if (!esc) {
            if (c=='\\') { esc=true; i++; continue; }
            if (c=='\'') { return { /*106=字符*/ 106, i+1 }; }
            if (c=='\n') { return { -2, i }; }
            i++;
        } else {
            esc=false; i++;
        }
    }
    return { -2, i };
}

/* ========================= 数字扫描（整数/浮点） ========================= */

enum NumKind { NK_DEC_INT=99, NK_OCT_INT=103, NK_HEX_INT=104, NK_BIN_INT=105, NK_FLOAT=101 };

ScanResult scanNumber(const char *s) {
    int i=0;
    // 0x / 0X
    if (s[i]=='0' && (s[i+1]=='x'||s[i+1]=='X')) {
        i+=2;
        int start=i;
        while (IsHex(s[i])) i++;
        if (i==start) return {-2, max(i,2)}; // 0x 后无数字
        return { NK_HEX_INT, i };
    }
    // 0b / 0B  (扩展)
    if (s[i]=='0' && (s[i+1]=='b'||s[i+1]=='B')) {
        i+=2;
        int start=i;
        while (s[i]=='0' || s[i]=='1') i++;
        if (i==start) return {-2, max(i,2)};
        if (IsLetter(s[i]) || IsDigit(s[i])) return {-2, i};
        return { NK_BIN_INT, i };
    }

    // 纯数字起步：十进制或八进制或浮点
    int j=i;
    while (IsDigit(s[j])) j++;
    bool isFloat=false;

    int k=j;
    if (s[k]=='.') {
        isFloat=true; k++;
        while (IsDigit(s[k])) k++;
    }
    // 指数
    int h=k;
    if (s[h]=='e' || s[h]=='E') {
        isFloat=true; h++;
        if (s[h]=='+'||s[h]=='-') h++;
        if (!IsDigit(s[h])) return {-2, h};
        while (IsDigit(s[h])) h++;
        return { NK_FLOAT, h };
    }
    if (isFloat) return { NK_FLOAT, k };

    // 非浮点：可能是八进制（0...）或十进制
    string v(s, j);
    if (v.size()>=2 && v[0]=='0') {
        for (size_t t=1;t<v.size();t++) {
            if (v[t]<'0' || v[t]>'7') return {-2, (int)j}; // 如 08
        }
        return { NK_OCT_INT, j };
    }
    return { NK_DEC_INT, j };
}

/* ======================= 运算符/界符匹配 ======================= */

bool matchStr(const char *s, const char *pat) {
    for (int i=0; pat[i]; i++) if (s[i]!=pat[i]) return false;
    return true;
}

bool matchAndTake(const char *s, const char *pat, int &advance) {
    if (matchStr(s, pat)) { advance = (int)strlen(pat); return true; }
    return false;
}

/* ============================ GetToken ============================ */
void GetToken(int &syn) {
    tokenLexeme[0]='\0';

    if (resourceProject[pProject]=='\0') { syn=0; return; }

    const char *s = resourceProject + pProject;

    // 跳过空格和换行（换行仅用于行号统计，词法不保留）
    while (*s==' ' || *s=='\n') { pProject++; s = resourceProject + pProject; if (*s=='\0') { syn=0; return; } }

    int startIdx = pProject;
    int startLine = dstLineOf[startIdx];
    int startCol  = dstColOf[startIdx];

    // 1) 字符串常量
    if (*s=='"') {
        auto res = scanStringLiteral(s);
        int take = res.len;
        strncpy(tokenLexeme, s, min(take, MAX_TOKEN-1));
        tokenLexeme[min(take, MAX_TOKEN-1)]='\0';
        pProject += take;
        syn = res.syn;
        if (syn==-2) {
            fprintf(fp1, "词法错误：第 %d 行，第 %d 列：字符串常量未闭合，位置附近为 \"%s\"\n",
                    startLine, startCol, tokenLexeme);
        }
        return;
    }

    // 2) 字符常量
    if (*s=='\'') {
        auto res = scanCharLiteral(s);
        int take = res.len;
        strncpy(tokenLexeme, s, min(take, MAX_TOKEN-1));
        tokenLexeme[min(take, MAX_TOKEN-1)]='\0';
        pProject += take;
        syn = res.syn;
        if (syn==-2) {
            fprintf(fp1, "词法错误：第 %d 行，第 %d 列：字符常量未闭合，位置附近为 '%s'\n",
                    startLine, startCol, tokenLexeme);
        }
        return;
    }

    // 3) 标识符 / 关键字
    if (IsLetter(*s)) {
        int i=0; while (IsLetter(s[i]) || IsDigit(s[i])) i++;
        strncpy(tokenLexeme, s, min(i, MAX_TOKEN-1));
        tokenLexeme[min(i, MAX_TOKEN-1)] = '\0';
        pProject += i;
        int kw = searchReserve(tokenLexeme);
        syn = (kw==-1 ? 100 : kw);
        return;
    }

    // 4) 数字
    if (IsDigit(*s)) {
        auto res = scanNumber(s);
        int take = res.len;
        strncpy(tokenLexeme, s, min(take, MAX_TOKEN-1));
        tokenLexeme[min(take, MAX_TOKEN-1)]='\0';
        pProject += take;
        syn = res.syn;
        if (syn==-2) {
            fprintf(fp1, "词法错误：第 %d 行，第 %d 列：数字格式错误，位置附近为 '%s'\n",
                    startLine, startCol, tokenLexeme);
        }
        return;
    }

    // 5) 复合赋值（优先于短运算符）
    {
        int adv=0;
        if (matchAndTake(s, "<<=", adv)) { syn=70; strncpy(tokenLexeme,s,adv); tokenLexeme[adv]='\0'; pProject+=adv; return; }
        if (matchAndTake(s, ">>=", adv)) { syn=71; strncpy(tokenLexeme,s,adv); tokenLexeme[adv]='\0'; pProject+=adv; return; }
        if (matchAndTake(s, "+=", adv))  { syn=72; strncpy(tokenLexeme,s,adv); tokenLexeme[adv]='\0'; pProject+=adv; return; }
        if (matchAndTake(s, "-=", adv))  { syn=73; strncpy(tokenLexeme,s,adv); tokenLexeme[adv]='\0'; pProject+=adv; return; }
        if (matchAndTake(s, "*=", adv))  { syn=74; strncpy(tokenLexeme,s,adv); tokenLexeme[adv]='\0'; pProject+=adv; return; }
        if (matchAndTake(s, "/=", adv))  { syn=75; strncpy(tokenLexeme,s,adv); tokenLexeme[adv]='\0'; pProject+=adv; return; }
        if (matchAndTake(s, "%=", adv))  { syn=76; strncpy(tokenLexeme,s,adv); tokenLexeme[adv]='\0'; pProject+=adv; return; }
        if (matchAndTake(s, "&=", adv))  { syn=77; strncpy(tokenLexeme,s,adv); tokenLexeme[adv]='\0'; pProject+=adv; return; }
        if (matchAndTake(s, "|=", adv))  { syn=78; strncpy(tokenLexeme,s,adv); tokenLexeme[adv]='\0'; pProject+=adv; return; }
        if (matchAndTake(s, "^=", adv))  { syn=79; strncpy(tokenLexeme,s,adv); tokenLexeme[adv]='\0'; pProject+=adv; return; }
    }

    // 6) 双字符运算符（++, --, ==, !=, >=, <=, &&, ||, <<, >>）
    {
        struct Pair { const char* s; int idx; };
        static const Pair pairs[] = {
            {"++",0}, {"--",1}, {"==",8}, {"!=",9}, {">=",12}, {"<=",13},
            {"&&",14}, {"||",15}, {"<<",21}, {">>",22}
        };
        int adv=0;
        for (auto &pr : pairs) {
            if (matchAndTake(s, pr.s, adv)) {
                strncpy(tokenLexeme,s,adv); tokenLexeme[adv]='\0';
                pProject+=adv;
                int synBase = OPDEL_BASE + pr.idx;
                syn = synBase;
                return;
            }
        }
    }

    // 7) 单字符运算符/界符
    {
        const string singles = "+-*/%=" "!><&|^~" "()[ ]{}" ",;.:?#";
        if (singles.find(*s) != string::npos) {
            char buf[2] = { *s, '\0' };
            int idx = -1;
            for (int i=0;i<OPDEL_CNT;i++) {
                if (strlen(operatorOrDelimiter[i])==1 &&
                    operatorOrDelimiter[i][0]==*s) { idx=i; break; }
            }
            if (idx>=0) {
                strcpy(tokenLexeme, buf);
                pProject++;
                syn = OPDEL_BASE + idx;
                return;
            }
        }
    }

    // 8) 其他不可识别字符：报错并跳过
    {
        char ch = *s;
        tokenLexeme[0]=ch; tokenLexeme[1]='\0';
        pProject++;
        syn = -2;
        fprintf(fp1, "词法错误：第 %d 行，第 %d 列：非法字符 '%c'\n",
                startLine, startCol, ch);
        return;
    }
}

/* ============================ 工具: 带行号输出（两位） ============================ */
void dump_with_line_numbers(FILE *out, const char *title, const char *text) {
    fprintf(out, "===== %s =====\n", title);
    int ln=1;
    fprintf(out, "%02d ", ln);
    for (int i=0;text[i]!='\0';i++) {
        fputc(text[i], out);
        if (text[i]=='\n' && text[i+1]!='\0') {
            ln++;
            fprintf(out, "%02d ", ln);
        }
    }
    fprintf(out, "\n===== 结束：%s =====\n\n", title);
    fflush(out);
}

/* ============================ 主程序 ============================ */
int main() {
    ios::sync_with_stdio(false);

    // 打开输入/输出（相对路径；如需绝对路径可自行修改）
    fp = fopen("D:\\桌面\\词法分析器\\testHard.c", "r");
    if (!fp) {
        cout << "无法打开\n";
        return 0;
    }
    fp1 = fopen("D:\\桌面\\词法分析器\\lex_out.txt", "w+");
    if (!fp1) {
        cout << "无法创建\n";
        fclose(fp);
        return 0;
    }

    // 读取源文件到 resourceProject（保留换行）
    int n = fread(resourceProject, 1, MAX_BUF-1, fp);
    resourceProject[n]='\0';
    fclose(fp);

    // 1) 原文带行号输出（中文）
    dump_with_line_numbers(fp1, (char*)"源程序", resourceProject);

    // 2) 过滤
    filterResource(resourceProject);

    // 过滤后的文本带行号输出（中文）
    dump_with_line_numbers(fp1, (char*)"过滤后源程序", resourceProject);

    // 3) 词法分析
    fprintf(fp1, "===== 词法单元 =====\n");
    pProject = 0;

    // 标识符表清空
    for (int i=0;i<MAX_ID;i++) IdentifierTbl[i][0]='\0';

    while (true) {
        int startIdx = pProject;
        int startLine = dstLineOf[startIdx];
        int startCol  = dstColOf[startIdx];

        int syn;
        GetToken(syn);
        if (syn==0) break;           // EOF
        if (syn==-2) continue;       // 错误已输出，跳过继续

        // 打印 token（中文）
        if (syn==100) {
            // 标识符入表（去重）
            bool exist=false; int idx=0;
            for (; idx<MAX_ID; idx++) {
                if (IdentifierTbl[idx][0]=='\0') break;
                if (strcmp(IdentifierTbl[idx], tokenLexeme)==0) { exist=true; break; }
            }
            if (!exist && idx<MAX_ID) strcpy(IdentifierTbl[idx], tokenLexeme);
            fprintf(fp1, "(标识符,%s)  @%d:%d\n", tokenLexeme, startLine, startCol);
        } else if (syn>=1 && syn<=32) {
            fprintf(fp1, "(关键字,%s)  @%d:%d\n", reserveWord[syn-1], startLine, startCol);
        } else if (syn==99) { // 十进制整数
            fprintf(fp1, "(十进制整数,%s)  @%d:%d\n", tokenLexeme, startLine, startCol);
        } else if (syn==101) { // 浮点
            fprintf(fp1, "(浮点数,%s)  @%d:%d\n", tokenLexeme, startLine, startCol);
        } else if (syn==103) {
            fprintf(fp1, "(八进制整数,%s)  @%d:%d\n", tokenLexeme, startLine, startCol);
        } else if (syn==104) {
            fprintf(fp1, "(十六进制整数,%s)  @%d:%d\n", tokenLexeme, startLine, startCol);
        } else if (syn==105) {
            fprintf(fp1, "(二进制整数,%s)  @%d:%d\n", tokenLexeme, startLine, startCol);
        } else if (syn==102) {
            fprintf(fp1, "(字符串常量,%s)  @%d:%d\n", tokenLexeme, startLine, startCol);
        } else if (syn==106) {
            fprintf(fp1, "(字符常量,%s)  @%d:%d\n", tokenLexeme, startLine, startCol);
        } else if (syn>=70 && syn<=79) {
            fprintf(fp1, "(复合赋值,%s)  @%d:%d\n", tokenLexeme, startLine, startCol);
        } else if (syn>=OPDEL_BASE && syn<OPDEL_BASE+OPDEL_CNT) {
            int idx = syn-OPDEL_BASE;
            fprintf(fp1, "(运算/界符,%s)  @%d:%d\n", operatorOrDelimiter[idx], startLine, startCol);
        } else {
            // 未知但非错误（兜底）
            fprintf(fp1, "(syn=%d,%s)  @%d:%d\n", syn, tokenLexeme, startLine, startCol);
        }
    }

    // 标识符表输出（中文）
    fprintf(fp1, "\n===== 标识符表 =====\n");
    for (int i=0;i<MAX_ID && IdentifierTbl[i][0]!='\0'; i++) {
        fprintf(fp1, "[%04d] %s\n", i+1, IdentifierTbl[i]);
    }

    fprintf(fp1, "\n===== 结束：词法单元 =====\n");

    // 过滤阶段检测到的“块注释未闭合”
    if (unterminatedCommentFound) {
        fprintf(fp1, "\n词法错误：块注释在文件结束前未闭合（/* ... */）\n");
    }

    // 输出被删除字符位置（原文）
    fprintf(fp1, "\n===== 被删除字符原始位置（行:列） =====\n");
    for (int k=0;k<removedCnt;k++) {
        fprintf(fp1, "- %d:%d\n", removedLine[k], removedCol[k]);
    }

    fclose(fp1);
    return 0;
}

