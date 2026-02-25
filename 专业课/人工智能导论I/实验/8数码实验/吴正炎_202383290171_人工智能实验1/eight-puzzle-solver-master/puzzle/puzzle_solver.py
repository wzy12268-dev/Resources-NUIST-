from utils.distance_metrics import manhattan_distance, eculidean_distance
from utils.search_algorithms import BFS, DFS, A_STAR
from puzzle.puzzle_state import PuzzleState
import math
import time
import sys
import os

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

        if self.search_alg == A_STAR:
            results = A_STAR(self.puzzle_state, self.calculate_total_cost)
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
