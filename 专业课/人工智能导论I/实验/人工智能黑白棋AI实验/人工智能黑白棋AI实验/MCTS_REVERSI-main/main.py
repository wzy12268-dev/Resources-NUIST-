import pygame
import sys
import random
import threading
from pygame.locals import *

from config import BLACK_TILE, WHITE_TILE, DIFFICULTY
from board import Board
from ai import MCTSAI   # ✅ 改这里！类名是 MCTSAI（没有下划线）
from gui import GameGUI


def who_goes_first():
    """随机决定谁先行"""
    if random.randint(0, 1) == 0:
        print("AI goes first")
        return 1
    else:
        print("Player goes first")
        return 0


def ai_move_thread(ai, board, computer_tile, gui, result_queue):
    """在单独线程中运行 AI 思考过程"""
    # ✅ 调用 get_best_move（与你的 ai.py 一致）
    move = ai.get_best_move(board, lambda progress: gui.update_ai_progress(progress))
    result_queue.append(move)


def main():
    """游戏主循环"""
    board = Board()
    gui = GameGUI()

    # 随机确定先手
    turn = who_goes_first()

    # 分配棋子颜色
    if turn == 0:  # 玩家先
        player_tile = BLACK_TILE
        computer_tile = WHITE_TILE
    else:  # AI 先
        player_tile = WHITE_TILE
        computer_tile = BLACK_TILE

    # ✅ 初始化 AI（与你的 ai.py 接口一致）
    ai = MCTSAI(difficulty=DIFFICULTY, player_color=computer_tile)

    game_over = False
    ai_thinking = False
    ai_move_result = []
    ai_thread = None

    while True:
        for event in pygame.event.get():
            if event.type == QUIT:
                if ai_thread and ai_thread.is_alive():
                    ai_thread.join(timeout=0.5)
                gui.quit()
                sys.exit()

            # 玩家点击落子
            if (not game_over and
                turn == 0 and
                event.type == MOUSEBUTTONDOWN and
                event.button == 1):

                col, row = gui.get_clicked_cell()
                if board.make_move(player_tile, col, row):
                    if board.get_valid_moves(computer_tile):
                        turn = 1
                    elif not board.get_valid_moves(player_tile):
                        game_over = True

            # Debug 快捷键：强制 AI 落子
            if event.type == KEYUP and event.key == K_q:
                turn = 1

        # === AI 回合 ===
        if not game_over and turn == 1:
            if not ai_thinking:
                ai_thinking = True
                gui.set_ai_thinking(True)
                ai_move_result.clear()

                ai_thread = threading.Thread(
                    target=ai_move_thread,
                    args=(ai, board, computer_tile, gui, ai_move_result)
                )
                ai_thread.daemon = True
                ai_thread.start()

            elif ai_move_result:
                move = ai_move_result[0]
                ai_thinking = False
                gui.set_ai_thinking(False)

                if move:
                    x, y = move
                    board.make_move(computer_tile, x, y)

                if board.get_valid_moves(player_tile):
                    turn = 0
                elif not board.get_valid_moves(computer_tile):
                    game_over = True

        # === 绘制 ===
        valid_moves = board.get_valid_moves(player_tile) if turn == 0 else []
        gui.draw_board(board, player_tile, valid_moves)
        gui.draw_player_indicators(player_tile, computer_tile)
        gui.draw_progress_bar()

        if game_over:
            scores = board.get_score()
            gui.draw_game_over(scores[player_tile], scores[computer_tile])

        gui.update_display()


if __name__ == '__main__':
    main()
