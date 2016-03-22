#!/usr/bin/env python
from ..model import Board

class Connect4Engine:

    def __init__(self, dimension, length_to_win):
        self.board = Board(dimension)
        self.length_to_win = length_to_win
        print self.board

    def place_token(self, player, column):
        if column < self.board.columns and column >= 0:
            open_row = self.open_space(column)
            if open_row > -1:
                self.board.grid[open_row][column] = player
                return player  # This player was successfully placed
        return -1  # The value should still be empty

    def winner(self):
        wins = []
        player_one_win = -1
        player_two_win = -1
        board_full = 0
        wins.append(self.check_winner_vertical())
        wins.append(self.check_winner_horizontal())
        wins.append(self.check_winner_diagonal_right_up())
        wins.append(self.check_winner_diagonal_left_up())
        board_full = self.check_full_board()
        for win in wins:
            if win == 0:
                player_one_win = 1
            if win == 1:
                player_two_win = 1
        if player_one_win == 1:
            return 0
        elif player_two_win == 1:
            return 1
        if board_full == self.board.columns:
            return 2
        return -1

    def check_winner_vertical(self):
        win_count = 0
        index_player = -1
        for column in xrange(0, self.board.columns - 1):
            for row in xrange(0, self.board.rows - 1):
                current_index = self.board.grid[row][column]
                if win_count == 0 and current_index != -1:
                    win_count += 1
                if current_index == self.board.grid[row - 1][column] and current_index != -1:
                    win_count += 1
                    index_player = self.board.grid[row][column]
                else:
                    win_count = 0
                    index_player = -1
                if win_count == self.length_to_win:
                    return index_player
        return -1

    def check_winner_horizontal(self):
        win_count = 0
        index_player = -1
        for row in xrange(0, self.board.rows - 1):
            for column in xrange(0, self.board.columns - 1):
                current_index = self.board.grid[row][column]
                if win_count == 0 and current_index != -1:
                    win_count += 1
                if current_index == self.board.grid[row][column + 1] and current_index != -1:
                    win_count += 1
                    index_player = self.board.grid[row][column]
                else:
                    win_count = 0
                    index_player = -1
                if win_count == self.length_to_win:
                    return index_player
        return -1

    def check_winner_diagonal_right_up(self):
        win_count = -1
        extra = 0
        rows = self.board.rows - 1
        max_diag = rows * 2 - 3
        for diag_left in xrange(2, max_diag):
            extra = 0
            if diag_left < rows:
                extra = 0
            else:
                extra = diag_left - rows + 1;
            win_count = -1
            for index in xrange(extra, (diag_left - extra)):
                row = index
                col = diag_left - index
                if win_count == 0 and self.board.grid[row][col]:
                    win_count += 1
                if self.board.grid[row - 1][col + 1] == self.board.grid[row][col] and self.board.grid[row][col] != -1:
                    win_count += 1
                else:
                    win_count = 0
                if win_count >= self.length_to_win:
                    return self.board.grid[row][col]
        return -1

    def check_winner_diagonal_left_up(self):
        win_count = -1
        extra = 0
        rows = self.board.rows
        max_diag = rows * 2 - 3
        for diag_right in xrange(2, max_diag):
            extra = 0
            if (diag_right < rows):
                extra = 0
            else:
                extra = diag_right - rows + 1
            win_count = -1
            for index in xrange(extra, diag_right - extra):
                row = index
                col = (rows - 1) - (diag_right - index)
                if ((win_count == 0) and (self.board.grid[row][col] != -1)):
                    win_count += 1
                if self.board.grid[row - 1][col - 1] == self.board.grid[row][col] and self.board.grid[row][col] != -1:
                    win_count += 1
                else:
                    win_count = 0
                if win_count >= self.length_to_win:
                    return self.board.grid[row][col]
        return -1

    def open_space(self, column):
        for row in xrange(self.board.rows - 1, -1, -1):
            print row, column
            current_index = self.board.grid[row][column]
            if current_index < 0:
                return row
        return -1  # No open spaces were found

    def check_full_board(self):
        full_count = 0  # The number of columns that are full
        for column in xrange(0, self.board.columns):
            if self.board.grid[0][column] != -1:
                full_count += 1
        return full_count
