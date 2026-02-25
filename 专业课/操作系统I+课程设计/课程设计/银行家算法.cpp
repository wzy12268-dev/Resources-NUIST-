#include <iostream>
using namespace std;

#define W 10   // 最大允许进程数
#define R 20   // 最大允许资源种类数

int M;  // 实际进程数 M <= W
int N;  // 实际资源种类数 N <= R

int ALL_RESOURCE[W];        // 各类资源的总数量
int AVAILABLE[R];           // 当前可用资源
int MAX_NEED[W][R];         // 最大需求矩阵 MAX
int ALLOCATION[W][R];       // 已分配矩阵 ALLOCATION
int NEED[W][R];             // 仍需矩阵 NEED = MAX - ALLOCATION
int Request[R];             // 某次请求

// 函数声明
void inputdata();
void showdata();
void try_allocate(int p);
void rollback_allocate(int p);
int  safety_check(int safeSeq[], int &outLen); // 返回安全? 并给出“仍在运行的进程”的执行顺序
void print_safe_seq(const char* msg);          // 使用新的 safety_check
void release_if_finished(int p);               // 若进程p需求清零则回收资源并标记为完成
void banker();                                 // 主交互流程

int main() {
    inputdata();

    // 启动前先检查系统初始是否安全
    int seq[W];
    int seqLen = 0;
    if (!safety_check(seq, seqLen)) {
        cout << "错误：系统初始状态不安全，不能进入资源分配流程！" << endl;
        return 0;
    } else {
        cout << "提示：系统初始状态安全。" << endl;
        print_safe_seq("初始安全序列");
        cout << endl;
    }

    banker(); // 进入交互循环
    return 0;
}

// ========== 初始化系统状态 ==========
void inputdata() {
    int i, j;

    cout << "请输入总进程数 M (不超过 " << W << "): ";
    do {
        cin >> M;
        if (M > W) {
            cout << "超过上限，请重新输入 M: ";
        }
    } while (M > W);
    cout << endl;

    cout << "请输入资源的种类数 N (不超过 " << R << "): ";
    do {
        cin >> N;
        if (N > R) {
            cout << "超过上限，请重新输入 N: ";
        }
    } while (N > R);
    cout << endl;

    cout << "请输入各类资源的总数量 ALL_RESOURCE[j]:" << endl;
    for (i = 0; i < N; i++) {
        cin >> ALL_RESOURCE[i];
    }
    cout << endl;

    cout << "请输入每个进程的最大需求矩阵 MAX_NEED[i][j]:" << endl;
    for (i = 0; i < M; i++) {
        cout << "进程 P" << i << " 的最大需求:" << endl;
        for (j = 0; j < N; j++) {
            while (true) {
                cin >> MAX_NEED[i][j];
                if (MAX_NEED[i][j] > ALL_RESOURCE[j]) {
                    cout << "  错：该值超过资源" << j << "的总量，请重新输入: ";
                } else {
                    break;
                }
            }
        }
    }
    cout << endl;

    cout << "请输入每个进程当前已分配的资源矩阵 ALLOCATION[i][j]:" << endl;
    for (i = 0; i < M; i++) {
        cout << "进程 P" << i << " 的已分配资源:" << endl;
        for (j = 0; j < N; j++) {
            while (true) {
                cin >> ALLOCATION[i][j];
                if (ALLOCATION[i][j] > MAX_NEED[i][j]) {
                    cout << "  错：已分配超过该进程声明的最大需求，请重新输入: ";
                } else {
                    break;
                }
            }
        }
    }
    cout << endl;

    // NEED = MAX - ALLOCATION
    for (i = 0; i < M; i++) {
        for (j = 0; j < N; j++) {
            NEED[i][j] = MAX_NEED[i][j] - ALLOCATION[i][j];
        }
    }

    // AVAILABLE = 总量 - sum(已分配)
    for (j = 0; j < N; j++) {
        int used = 0;
        for (i = 0; i < M; i++) {
            used += ALLOCATION[i][j];
        }
        AVAILABLE[j] = ALL_RESOURCE[j] - used;
        if (AVAILABLE[j] < 0) AVAILABLE[j] = 0;
    }

    cout << "初始资源状态如下：" << endl;
    showdata();
}

// ========== 打印当前资源状态 ==========
void showdata() {
    int i, j;

    cout << "----------------------------------------" << endl;
    cout << "资源总量 ALL_RESOURCE:" << endl << "  ";
    for (j = 0; j < N; j++) {
        cout << "R" << j << "=" << ALL_RESOURCE[j] << "   ";
    }
    cout << "\n\n";

    cout << "当前可用 AVAILABLE:" << endl << "  ";
    for (j = 0; j < N; j++) {
        cout << "R" << j << "=" << AVAILABLE[j] << "   ";
    }
    cout << "\n\n";

    cout << "NEED[i][j]:" << endl;
    for (i = 0; i < M; i++) {
        cout << "  P" << i << ": ";
        for (j = 0; j < N; j++) {
            cout << NEED[i][j] << "\t";
        }
        cout << endl;
    }
    cout << "\n";

    cout << "ALLOCATION[i][j] (已分配):" << endl;
    for (i = 0; i < M; i++) {
        cout << "  P" << i << ": ";
        for (j = 0; j < N; j++) {
            cout << ALLOCATION[i][j] << "\t";
        }
        cout << endl;
    }
    cout << "----------------------------------------" << endl;
}

// ========== 试分配（假设允许这次请求） ==========
void try_allocate(int p) {
    for (int j = 0; j < N; j++) {
        AVAILABLE[j]     -= Request[j];
        ALLOCATION[p][j] += Request[j];
        NEED[p][j]       -= Request[j];
    }
}

// ========== 回滚分配（如果最后不安全） ==========
void rollback_allocate(int p) {
    for (int j = 0; j < N; j++) {
        AVAILABLE[j]     += Request[j];
        ALLOCATION[p][j] -= Request[j];
        NEED[p][j]       += Request[j];
    }
}

// ========== 安全性检查 ==========
// 1. 照银行家算法判断“系统是否安全”：也就是有没有办法让所有存活的进程都依次完成。
// 2. 生成一个顺序 safeSeqAll[] 表示可能的完成顺序（包含所有还没 finish 的进程和那些已经完成的也会被标记）。
// 3. 我们再过滤掉“已经完成并回收资源的进程”，只输出当前仍活跃的进程顺序。
//    我们把“已经完成”的判定标准定义为：该进程 MAX_NEED[i][j] 全部为0 且 ALLOCATION[i][j]为0 且 NEED[i][j]为0。
// 参数：
//   safeSeq[]  - 输出过滤后的安全序列（也就是你关心的，仍未结束的进程）
//   outLen     - 输出序列的长度
// 返回值：
//   1 表示系统安全（存在安全序列），0 表示不安全
int safety_check(int safeSeq[], int &outLen) {
    int work[R];
    int finish[W];
    int finishedAlready[W];

    // 判断哪些进程其实已经“完全结束”了
    for (int i = 0; i < M; i++) {
        bool ended = true;
        for (int j = 0; j < N; j++) {
            if (MAX_NEED[i][j] != 0 || ALLOCATION[i][j] != 0 || NEED[i][j] != 0) {
                ended = false;
                break;
            }
        }
        finishedAlready[i] = ended ? 1 : 0;
    }

    // work = AVAILABLE
    for (int j = 0; j < N; j++) {
        work[j] = AVAILABLE[j];
    }

    // init finish[]
    for (int i = 0; i < M; i++) {
        // 如果进程已经结束了，那么我们可以视为它“已经完成”，不需要再考虑它是否还能跑
        finish[i] = finishedAlready[i] ? 1 : 0;
    }

    // 我们构建一个顺序列表，把所有能完成的进程按完成次序推入
    int orderAll[W];
    int countAll = 0;

    bool progress = true;
    while (progress) {
        progress = false;

        for (int i = 0; i < M; i++) {
            if (!finish[i]) {
                bool canFinish = true;
                for (int j = 0; j < N; j++) {
                    if (NEED[i][j] > work[j]) {
                        canFinish = false;
                        break;
                    }
                }
                if (canFinish) {
                    // 模拟它完成：释放它的资源
                    for (int j = 0; j < N; j++) {
                        work[j] += ALLOCATION[i][j];
                    }
                    finish[i]   = 1;
                    orderAll[countAll++] = i;
                    progress = true;
                }
            }
        }
    }

    // 检查是否所有进程都 finish（包括那些原本就结束的）
    for (int i = 0; i < M; i++) {
        if (!finish[i]) {
            // 还有没法完成的，说明系统不安全
            outLen = 0;
            return 0;
        }
    }

    // 现在我们需要把 orderAll[] 过滤一下，只保留“还没结束”的进程
    // 也就是说，finishedAlready[i] == 0 的那些
    outLen = 0;
    for (int idx = 0; idx < countAll; idx++) {
        int pid = orderAll[idx];
        if (!finishedAlready[pid]) {
            safeSeq[outLen++] = pid;
        }
    }

    // 如果所有进程都已经 finishedAlready==1（也就是系统里活跃进程都结束了）
    // 那 outLen 会是0，这没关系，表示当前没有还需要资源的活跃进程。
    return 1;
}

// ========== 打印当前系统的安全序列（使用过滤后序列） ==========
void print_safe_seq(const char* msg) {
    int seq[W];
    int seqLen = 0;
    if (!safety_check(seq, seqLen)) {
        cout << msg << "：当前状态不安全（不存在安全序列）" << "\n";
        return;
    }

    cout << msg << "：";
    if (seqLen == 0) {
        cout << "(无剩余未完成进程，系统资源全部可回收)";
    } else {
        for (int i = 0; i < seqLen; i++) {
            cout << "P" << seq[i];
            if (i < seqLen - 1) cout << " -> ";
        }
    }
    cout << "\n";
}

// ========== 如果进程 p 的需求全是0，释放它占有的资源并标记为完全结束 ==========
void release_if_finished(int p) {
    bool done = true;
    for (int j = 0; j < N; j++) {
        if (NEED[p][j] != 0) {
            done = false;
            break;
        }
    }

    if (done) {
        cout << "进程 P" << p << " 已满足全部资源需求，视为执行完成，系统回收其占用资源。" << endl;
        for (int j = 0; j < N; j++) {
            AVAILABLE[j]     += ALLOCATION[p][j];
            ALLOCATION[p][j]  = 0;
            NEED[p][j]        = 0;
            MAX_NEED[p][j]    = 0; // 关键：标记该进程“真正结束”
        }
    }
}

// ========== 银行家算法主交互循环 ==========
void banker() {
    char cont = 'Y';

    while (cont == 'Y' || cont == 'y') {

        // 1. 选择要申请资源的进程号
        int pid = -1;
        while (pid < 0 || pid >= M) {
            cout << "请输入申请资源的进程号(0~" << M - 1 << "): P";
            cin >> pid;
            if (pid < 0 || pid >= M) {
                cout << "进程号无效，请重新输入。" << endl;
            }
        }

        // 2. 输入该进程对每类资源的请求
        bool valid = true;
        cout << "请输入进程 P" << pid << " 本次申请的各类资源数量:" << endl;
        for (int j = 0; j < N; j++) {
            cout << "  资源" << j << ": ";
            cin >> Request[j];

            if (Request[j] > NEED[pid][j]) {
                cout << "  错：请求量超过进程 P" << pid
                     << " 对资源" << j << " 的尚需数量！" << endl;
                valid = false;
            } else if (Request[j] > AVAILABLE[j]) {
                cout << "  错：请求量超过系统当前可用的资源" << j << " 数量！" << endl;
                valid = false;
            }
        }

        if (!valid) {
            cout << "\n该请求不合法，本次不分配资源。\n" << endl;
        } else {
            // 3. 试分配
            try_allocate(pid);

            // 4. 做一次安全性检查（用新的 safety_check）
            int seqTmp[W];
            int lenTmp = 0;
            if (!safety_check(seqTmp, lenTmp)) {
                cout << "\n如果进行该分配，系统将变得不安全，拒绝本次申请！" << endl;
                // 回滚
                rollback_allocate(pid);
            } else {
                cout << "\n系统仍处于安全状态，分配成功！" << endl;
                print_safe_seq("分配后安全序列(剩余未完成进程)");

                // 显示分配后的状态
                cout << "\n分配后资源状态：" << endl;
                showdata();

                // 5. 检查这个进程是不是已经拿齐所有资源，如果是就释放
                release_if_finished(pid);

                // 6. 回收之后再次显示状态 + 再算并打印新的安全序列
                cout << "回收后资源状态：" << endl;
                showdata();

                print_safe_seq("回收后安全序列(剩余未完成进程)");
                cout << endl;
            }
        }

        // 7. 是否继续
        cout << "是否继续申请资源？(Y继续 / N结束): ";
        cin >> cont;
        cout << endl;
    }

    cout << "演示结束。" << endl;
}

