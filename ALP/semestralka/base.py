import copy

""" base class for LITS player.

    *** DO NOT MODIFY THIS FILE ***

"""


class BasePlayer:
    def __init__(self, name, board, marks, stones, player):
        """ constructor of BasePlayer class with creation of some usefull variables """

        self.name = name  # name of your player (usually, your KOS/email name)
        self.board = copy.deepcopy(board)  # board - 2D array, accessing by self.board[row][col]
        self.marks = copy.deepcopy(marks)  # marks - 2D array, accessing by self.marks[row][col]
        self.stones = copy.deepcopy(stones)  # list of stones. Same format as in HW08
        self.player = player  # if self.player==1, you player for 'o', if self.player==-1, you play for 'x'
        self.freeStones = [True] * len(stones)  # 1D array of True/False. if self.freeStones[i] == True, the i-the stone is available
        self.algorithm = "none"  # for tournament mode. Name of your algorithm. Set it in Player class
        self.tournament = False  # True in tournament mode.

    def score(self, player):
        """ count the number of open pieces for player (=1 or -1) 
            self.score(self.player) -> counts YOUR score (the number of your open marks)
            self.score(-self.player) -> counts OPPONENT's score (the number of his/her open marks)
        """
        cnt = 0
        for row in range(len(self.board)):
            for col in range(len(self.board[row])):
                if self.marks[row][col] == player and self.board[row][col] == 0:
                    cnt += 1
        return cnt

    def printb(self):
        """ simple print of actual status of the board """
        for col in range(len(self.board[0])):
            print("----", end="")
        print("-")
        for row in range(len(self.board)):
            print("|", end="")
            for col in range(len(self.board[row])):
                print("{:3d}|".format(self.board[row][col]), end="")
            print()
            for col in range(len(self.board[0])):
                print("----", end="")
            print("-")

    def inBoard(self, row, col):
        """ true if coordinate [row][col] is within the board """
        return row >= 0 and row < len(self.board) and col >= 0 and col < len(self.board[0])

    def isEmpty(self):
        """ true if the board is empty. Useful e.g. to determine the first move """
        for row in range(len(self.board)):
            for col in range(len(self.board)):
                if self.board[row][col] != 0:
                    return False
        return True


# ==============================================================


def loadStones(filename):
    """ load stones from text file. Stones are stored in the same format as in HW08 """
    f = open(filename, "r")
    stones = []
    for line in f:
        nums = list(map(int, line.strip().split()))
        color = nums[0]
        coords = nums[1:]
        cells = []
        for i in range(0, len(coords) // 2):
            cells.append([coords[2 * i + 0], coords[2 * i + 1]])
        stones.append([color, cells])
    f.close()
    return stones


def makeBoard10():
    """ create example of 10x10 board and corresponding marks """
    board = []
    marks = []
    size = 10
    for i in range(size):
        board.append([0] * size)
        marks.append([0] * size)

    cnt = 0
    fig = 1
    coords = []
    for col in range(len(board[0])):
        coords.append([0, col])
    for row in range(1, len(board)):
        coords.append([row, len(board[0]) - 1])
    for col in range(len(board[0]) - 2, -1, -1):
        coords.append([len(board) - 1, col])
    for row in range(len(board) - 2, 0, -1):
        coords.append([row, 0])
    for c in coords:
        row, col = c
        marks[row][col] = fig
        cnt += 1
        if cnt == 3:
            cnt = 0
            fig = -fig

    marks[3][3] = 1
    marks[4][4] = 1
    marks[5][5] = 1
    marks[6][6] = 1
    marks[3][6] = -1
    marks[4][5] = -1
    marks[5][4] = -1
    marks[6][3] = -1

    return board, marks


def writeBoard(board, stone, color):
    """ write stone into board using given color.
        board .. 2D array (e.g. Player.board), 
        stone [ [ row1,col1], ... [rown],[coln] ]
        color .. integer .. color of the stone
    """
    for i in range(len(stone)):
        row, col = stone[i]
        board[row][col] = color
