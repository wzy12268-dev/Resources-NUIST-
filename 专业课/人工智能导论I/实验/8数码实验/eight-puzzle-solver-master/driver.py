from puzzle.puzzle import PuzzleSolver
import sys

if __name__ == '__main__':
    chosen_heuristic = None
    chosen_algorithm = sys.argv[1].lower()  # 读取算法：bfs / dfs / ast
    begin_state = sys.argv[2].split(",")  # 读取初始状态（逗号分隔的 9 个数）
    if(chosen_algorithm == 'ast'):
        value = input("Please choose a heuristic fucntion:\n[1] Manhattan Distance  [2] Euclidean Distance  ")
        if(value == str(1)):
            chosen_heuristic = "manhattan"
        elif(value == str(2)):
            chosen_heuristic = "euclidean"
        else: 
            raise Exception("Wrong input heuristic function !") 
        
    solver = PuzzleSolver(begin_state,[0,1,2,3,4,5,6,7,8], chosen_algorithm, heuristic=chosen_heuristic)
    if len(sys.argv) >= 4 and sys.argv[3].lower().endswith(".txt"):
        solver.log_path = sys.argv[3]
    solver.solve()
    # python driver.py dfs 1,2,0,3,4,5,6,7,8 0,1,2,3,4,5,6,7,8 dfs.txt
    # python D:\Pycharm\eight-puzzle-solver-master\driver.py bfs 1,5,2,3,0,4,6,8,7 0,1,2,3,4,5,6,7,8
    # python D:\Pycharm\eight-puzzle-solver-master\driver.py ast 1,2,3,4,5,6,8,7,0 0,1,2,3,4,5,6,7,8 manhattan