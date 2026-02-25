from utils.distance_metrics import manhattan_distance, eculidean_distance
from utils.search_algorithms import BFS, DFS, A_STAR
from puzzle.puzzle_state import PuzzleState
import math
import time
import sys
import os
import heapq
from datetime import datetime

# -------- Cross-platform memory usage helpers --------
try:
    import resource  # Unix-like only
except Exception:
    resource = None

try:
    import psutil  # pip install psutil
except Exception:
    psutil = None


def _rss_kb_now():
    """
    Return current process RSS in KB if possible; None if unavailable.
    Linux: resource.ru_maxrss is KB; macOS: ru_maxrss is bytes -> convert.
    Windows: use psutil as fallback.
    """
    # Prefer resource on non-Windows
    if resource is not None and sys.platform != "win32":
        rss = resource.getrusage(resource.RUSAGE_SELF).ru_maxrss
        # macOS peculiarity: ru_maxrss is bytes
        if sys.platform == "darwin":
            rss = rss // 1024
        return int(rss)

    # Fallback: psutil
    if psutil is not None:
        return int(psutil.Process(os.getpid()).memory_info().rss // 1024)

    # Last resort
    return None
# -----------------------------------------------------

# 求解器
class PuzzleSolver(object):

    def __init__(self, initial_state, goal, algorithm='bfs', heuristic=None):
        self.initial_state = initial_state

        # Assign the search algorithm that will be used in the solver.
        if algorithm == 'bfs':
            self.search_alg = BFS
        elif algorithm == 'dfs':
            self.search_alg = DFS
        elif algorithm == 'ast':
            self.search_alg = A_STAR
        else:
            raise NotImplementedError("No such algorithm is supported.")

        # Astar必须提供启发式函数
        # Assign the heuristic algorithm that will be used in the solver.
        if (heuristic is None and algorithm == 'ast'):
            raise AttributeError("Required Attribute `heuristic` in case of useing A* Search.")
        elif heuristic == 'manhattan':
            self.dist_metric = manhattan_distance
        elif heuristic == 'euclidean':
            self.dist_metric = eculidean_distance
        elif (heuristic is None and algorithm != 'ast'):
            pass
        else:
            raise NotImplementedError("No such Heuristic is supported.")

        # Create a Puzzle State Object with the inputs for Solver.
        initial_state = tuple(map(int, initial_state))
        size = int(math.sqrt(len(initial_state)))
        self.puzzle_state = PuzzleState(initial_state, size, goal, self.calculate_total_cost)

        # 检查是否有解
        if not self.puzzle_state.is_solvable():
            raise Exception("The initial state enetred is not solvable !")

    # 遍历棋盘，按照距离公式计算所有小块到目标格的总距离->输出步数+距离和
    def calculate_total_cost(self, state):
        """calculate the total estimated cost of a state"""
        sum_heuristic = 0
        for i, item in enumerate(state.config):
            current_row = i // state.n
            current_col = i % state.n
            goal_idx = state.goal.index(item)
            goal_row = goal_idx // state.n
            goal_col = goal_idx % state.n
            sum_heuristic += self.dist_metric(current_row, current_col, goal_row, goal_col)
        return sum_heuristic + state.cost

    def writeOutput(self, result, running_time, ram_usage):
        final_state, nodes_expanded, max_search_depth = result
        path_to_goal = [final_state.action]
        cost_of_path = final_state.cost
        parent_state = final_state.parent

        while parent_state:
            if parent_state.parent:
                path_to_goal.append(parent_state.action)
            parent_state = parent_state.parent
        path_to_goal.reverse()
        search_depth = len(path_to_goal)

        print("******* Results *******")
        print("path_to_goal: " + str(path_to_goal) + "\n")
        print("cost_of_path: " + str(cost_of_path) + "\n")
        print("nodes_expanded: " + str(nodes_expanded) + "\n")
        print("search_depth: " + str(search_depth) + "\n")
        print("max_search_depth: " + str(max_search_depth) + "\n")
        print("running_time: " + str(running_time) + "\n")
        print("max_ram_usage: " + str(ram_usage) + "\n")

    def solve(self):
        start_time = time.time()
        mem_init = _rss_kb_now()

        # 如果是 A* 就用带日志版本；否则按原算法
        if self.search_alg == A_STAR:
            # 若 driver 传入了 log_path（见下文），优先用；否则默认文件名
            log_path = getattr(self, "log_path", None) or "astar_frontier_log.txt"
            results = self.A_STAR_with_logging(self.puzzle_state, self.calculate_total_cost, log_path)
        else:
            results = self.search_alg(self.puzzle_state)

        running_time = time.time() - start_time
        mem_final = _rss_kb_now()

        # 兼容：拿不到内存数据时给 None；否则换算成 MB
        if mem_init is None or mem_final is None:
            ram_usage = None
        else:
            ram_usage = (mem_final - mem_init) / 1024.0  # MB

        self.writeOutput(results, running_time, ram_usage)

    def _board_lines_with_hash(self, state):
        """把棋盘转成字符行，用 '#' 表示 0（空格）"""
        lines = []
        n = state.n
        for r in range(n):
            row = []
            for c in range(n):
                v = state.config[r * n + c]
                row.append('#' if v == 0 else str(v))
            lines.append(' '.join(row))
        return lines

    def _split_f_to_gh(self, state):
        """计算 g, h, f（与 calculate_total_cost 保持一致：包括对 0 的距离）"""
        g = state.cost
        # 直接用 calculate_total_cost 得到 f，再反推 h
        f = self.calculate_total_cost(state)
        h = f - g
        return g, h, f

    def A_STAR_with_logging(self, initial_state, heuristic, log_path="astar_frontier_log.txt"):
        """
        A* 搜索（与原 A* 逻辑等价），但在每次迭代把 frontier 全量写到 log_path。
        frontier 中每个结点输出其棋盘（# 代表空格）以及 g,h,f。
        """
        # open：最小堆，键=f(n)
        counter = 0  # 平手打破器，确保堆项可比
        open_heap = []
        heapq.heappush(open_heap, (heuristic(initial_state), counter, initial_state))
        counter += 1

        # 记录 open 里有哪些配置，以及它们当前的最优 f
        open_best_f = {initial_state.config: heuristic(initial_state)}
        explored = set()  # 关闭表（已扩展）
        nodes_expanded = 0
        max_search_depth = 0

        # 打开日志文件（覆盖写）
        with open(log_path, "w", encoding="utf-8") as fout:
            fout.write(f"# A* Frontier Trace  ({datetime.now().strftime('%Y-%m-%d %H:%M:%S')})\n")
            fout.write(f"# begin = {initial_state.config}, goal = {tuple(initial_state.goal)}\n\n")

            iter_id = 0
            while open_heap:
                # ---- 迭代开始：把当前 frontier 完整打印出来 ----
                # 注意：堆是无序结构，这里复制并按 (f, tie) 排序后打印，便于阅读
                snapshot = sorted(open_heap, key=lambda x: (x[0], x[1]))
                fout.write(f"=== Iteration {iter_id} | frontier_size={len(snapshot)} ===\n")
                for f_val, _, st in snapshot:
                    g, h, f = self._split_f_to_gh(st)
                    fout.write(f"- state config: {st.config}\n")
                    for line in self._board_lines_with_hash(st):
                        fout.write(f"  {line}\n")
                    fout.write(f"  g={g}, h={h}, f={f}\n\n")
                fout.write("\n")
                iter_id += 1
                # ---------------------------------------------------

                # 取出 f 最小的结点
                _, _, state = heapq.heappop(open_heap)
                # 丢弃已失效（open_best_f里有更优f）的旧条目
                f_now = heuristic(state)
                if open_best_f.get(state.config, None) is None or f_now != open_best_f[state.config]:
                    # 这是一个过期项，跳过
                    continue

                explored.add(state.config)

                # 命中目标
                if state.is_goal():
                    return (state, nodes_expanded, max_search_depth)

                nodes_expanded += 1

                # 展开后继（A* 使用 UDLR）
                for neighbor in state.expand(RLDU=False):
                    if neighbor.config in explored:
                        continue

                    g, h, f = self._split_f_to_gh(neighbor)
                    # 更新最深搜索深度
                    if neighbor.cost > max_search_depth:
                        max_search_depth = neighbor.cost

                    # 只在以下两种情况下入堆/更新：
                    # 1) 该配置从未进过 open；2) 发现更小的 f（更优）
                    old_best = open_best_f.get(neighbor.config)
                    if (old_best is None) or (f < old_best):
                        open_best_f[neighbor.config] = f
                        heapq.heappush(open_heap, (f, counter, neighbor))
                        counter += 1

        # 如果 open 为空则失败（理论上 8-puzzle 可解时不会发生）
        return (initial_state, nodes_expanded, max_search_depth)
    # ---- 新增方法结束 ----

