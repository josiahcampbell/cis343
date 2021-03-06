#!/usr/bin/env python

class Board:
    
    def __init__(self, rows, columns):
        self.rows = rows
        self.columns = columns
        self.grid = [[-1 for x in xrange(self.columns)] for y in xrange(self.rows)]

    def __str__(self):
        print_board = ""
        for i in xrange(0, self.rows):
            for j in xrange(0, self.columns):
                if self.grid[i][j] == -1:
                    print_board += ".  "
                else:
                    print_board += str(self.grid[i][j]) + "  "
            print_board += "\n"

        return print_board
