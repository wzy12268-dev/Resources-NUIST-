
import math
import random
import sys
from board import Board
from config import *

def other(color):
    return WHITE_TILE if color == BLACK_TILE else BLACK_TILE

class Node:
    """Represents a node in the MCTS tree."""
    def __init__(self, board, parent=None, action=None, color=None):
        self.board = board  # a Board instance (should be a copy when used in tree)
        self.parent = parent
        self.action = action  # action that led to this node (x,y)
        self.color = color  # the player who made the move to reach this node
        self.children = {}  # map action -> Node
        self.visits = 0
        self.reward = 0.0  # cumulative reward (from AI perspective, sign handled in backprop)

    def is_fully_expanded(self):
        # A node is fully expanded if number of children equals number of valid moves for next player
        next_color = other(self.color) if self.color is not None else BLACK_TILE
        valid = self.board.get_valid_moves(next_color)
        return len(valid) == len(self.children) and len(valid) > 0

    def ucb(self, c_param=1.414):
        # UCB value relative to parent; if unvisited return +inf to force exploration
        if self.parent is None:
            return float('inf')
        if self.visits == 0:
            return float('inf')
        parent_visits = self.parent.visits if self.parent else 1
        exploitation = self.reward / self.visits
        exploration = c_param * math.sqrt(math.log(parent_visits) / self.visits)
        return exploitation + exploration

class MCTSAI:
    def __init__(self, difficulty=200, player_color=BLACK_TILE):
        self.difficulty = difficulty  # iterations per move
        self.ai_color = player_color
        self.player_color = other(player_color)

    def get_best_move(self, board, progress_callback=None):
        root_board = board.get_copy()
        root = Node(root_board, parent=None, action=None, color=other(self.ai_color))  # color = last mover (opponent) so next is ai
        for it in range(self.difficulty):
            node = self._select(root)
            node_to_sim = self._expand(node)
            reward = self._simulate(node_to_sim)
            self._backpropagate(node_to_sim, reward)
            if progress_callback and (it % max(1, self.difficulty//20) == 0):
                try:
                    progress_callback(it / self.difficulty)
                except Exception:
                    pass

        # choose child with highest visits
        if not root.children:
            return None
        best = max(root.children.values(), key=lambda n: n.visits)
        return best.action

    def _select(self, node):
        # If node is a leaf (no children), select it for expansion/simulation
        if not node.children:
            return node
        # If fully expanded, pick child with max UCB and recurse
        if node.is_fully_expanded():
            best = max(node.children.values(), key=lambda c: c.ucb())
            return self._select(best)
        # Partially expanded: pick a random unvisited child to reduce bias
        unvisited = [c for c in node.children.values() if c.visits == 0]
        if unvisited:
            return random.choice(unvisited)
        # Fallback: select child with max ucb
        best = max(node.children.values(), key=lambda c: c.ucb())
        return self._select(best)

    def _expand(self, node):
        # If node has never been visited, we perform simulation at node itself
        if node.visits == 0:
            return node
        # Otherwise expand one previously-unadded child (typical MCTS expansion: add one child)
        next_color = other(node.color) if node.color is not None else BLACK_TILE
        valid = node.board.get_valid_moves(next_color)
        # find actions not yet in children
        unadded = [mv for mv in valid if mv not in node.children]
        if not unadded:
            # nothing to add -> return node (leaf)
            return node
        action = random.choice(unadded)
        new_board = node.board.get_copy()
        new_board.make_move(next_color, action[0], action[1])
        child = Node(new_board, parent=node, action=action, color=next_color)
        node.children[action] = child
        return child

    def _simulate(self, node):
        # Perform a heuristic-biased playout from node.board
        sim_board = node.board.get_copy()
        # whose turn is next? if node.color is the player who moved to reach node,
        # then next to move is the opposite
        current_color = other(node.color) if node.color is not None else BLACK_TILE
        # play until end or safety cap
        steps = 0
        while not sim_board.is_game_over() and steps < 200:
            valid = sim_board.get_valid_moves(current_color)
            if valid:
                mv = self._heuristic_play(sim_board, current_color, valid)
                sim_board.make_move(current_color, mv[0], mv[1])
            # switch player
            current_color = other(current_color)
            steps += 1

        scores = sim_board.get_score()
        ai_score = scores[self.ai_color]
        opp_score = scores[self.player_color]
        # normalized reward in [-1,1]
        reward = (ai_score - opp_score) / (BOARD_SIZE * BOARD_SIZE)
        return reward

    def _heuristic_play(self, board, color, valid_moves):
        # Prefer corners, then moves that maximize immediate score gain, then edges, else random
        corners = [(0,0),(0,BOARD_SIZE-1),(BOARD_SIZE-1,0),(BOARD_SIZE-1,BOARD_SIZE-1)]
        corner_moves = [m for m in valid_moves if m in corners]
        if corner_moves:
            return random.choice(corner_moves)
        # evaluate by immediate score gain (simulate move on a copy)
        best_gain = None
        best_moves = []
        base_score = board.get_score()[color]
        for m in valid_moves:
            tmp = board.get_copy()
            tmp.make_move(color, m[0], m[1])
            new_score = tmp.get_score()[color]
            gain = new_score - base_score
            if best_gain is None or gain > best_gain:
                best_gain = gain
                best_moves = [m]
            elif gain == best_gain:
                best_moves.append(m)
        if best_moves and best_gain is not None and best_gain > 0:
            return random.choice(best_moves)
        # prefer edges
        edge_moves = [m for m in valid_moves if m[0] in (0, BOARD_SIZE-1) or m[1] in (0, BOARD_SIZE-1)]
        if edge_moves:
            return random.choice(edge_moves)
        return random.choice(valid_moves)

    def _backpropagate(self, node, reward):
        # propagate reward up to root. reward is from AI perspective (can be negative)
        cur = node
        while cur is not None:
            cur.visits += 1
            # if the node.color is the AI who made the move to reach this node, add reward;
            # otherwise subtract so that nodes are valued from correct player's perspective
            if cur.color == self.ai_color:
                cur.reward += reward
            else:
                cur.reward -= reward
            cur = cur.parent

if __name__ == "__main__":
    import pygame
    import sys
    import time
    from datetime import datetime
    from board import Board
    from config import BLACK_TILE, WHITE_TILE, DIFFICULTY
    from gui import GameGUI

    # --- 初始化部分 ---
    pygame.init()
    board = Board()
    gui = GameGUI()

    # 创建两个AI
    ai_black = MCTSAI(difficulty=DIFFICULTY, player_color=BLACK_TILE)
    ai_white = MCTSAI(difficulty=DIFFICULTY, player_color=WHITE_TILE)

    turn = BLACK_TILE
    game_over = False

    # === 日志文件追加模式 ===
    log_path = "ai_selfplay_log.txt"
    with open(log_path, "a", encoding="utf-8") as f:
        f.write("\n" + "=" * 50 + "\n")
        f.write("==== 新的一局 AI 自我博弈开始 ====\n")
        f.write(f"开始时间: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}\n")
        f.write(f"AI黑方难度: {DIFFICULTY}, AI白方难度: {DIFFICULTY}\n\n")

    print("开始 AI 自我博弈演示...")
    print("黑方：AI1（MCTS）")
    print("白方：AI2（MCTS）")

    move_count = 1

    # --- 主博弈循环 ---
    while not game_over:
        gui.draw_board(board, BLACK_TILE, [])
        gui.update_display()
        pygame.event.pump()
        time.sleep(0.4)

        # 当前AI执行走棋
        if turn == BLACK_TILE:
            move = ai_black.get_best_move(board)
            if move:
                x, y = move
                board.make_move(BLACK_TILE, x, y)
                move_info = f"第{move_count:02d}步：黑方落子 ({x}, {y})"
            else:
                move_info = f"第{move_count:02d}步：黑方无子可下"
        else:
            move = ai_white.get_best_move(board)
            if move:
                x, y = move
                board.make_move(WHITE_TILE, x, y)
                move_info = f"第{move_count:02d}步：白方落子 ({x}, {y})"
            else:
                move_info = f"第{move_count:02d}步：白方无子可下"

        print(move_info)
        with open(log_path, "a", encoding="utf-8") as f:
            f.write(move_info + "\n")

        move_count += 1

        if (not board.get_valid_moves(BLACK_TILE)) and (not board.get_valid_moves(WHITE_TILE)):
            game_over = True
        else:
            turn = WHITE_TILE if turn == BLACK_TILE else BLACK_TILE

    # --- 游戏结束 ---
    gui.draw_board(board, BLACK_TILE, [])
    gui.update_display()

    scores = board.get_score()
    result_text = (
        "\n==== 对局结束 ====\n"
        f"黑方得分: {scores[BLACK_TILE]}, 白方得分: {scores[WHITE_TILE]}\n"
    )

    if scores[BLACK_TILE] > scores[WHITE_TILE]:
        result_text += "最终结果：黑方胜\n"
    elif scores[BLACK_TILE] < scores[WHITE_TILE]:
        result_text += "最终结果：白方胜\n"
    else:
        result_text += "最终结果：平局\n"

    print(result_text)

    with open(log_path, "a", encoding="utf-8") as f:
        f.write(result_text)
        f.write(f"结束时间: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}\n")

    gui.draw_game_over(scores[BLACK_TILE], scores[WHITE_TILE])
    gui.update_display()

    # 等待关闭窗口
    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()
