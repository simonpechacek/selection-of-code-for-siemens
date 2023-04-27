import base
import time
from draw import Drawer


# from player_stupid_old import Player2

class Player(base.BasePlayer):
    def __init__(self, name, board, marks, stones, player):
        """ constructor of Player. Place you variables here with prefix 'self' -> e.g. 'self.myVariable' """

        base.BasePlayer.__init__(self, name, board, marks, stones, player)  # do not change this line!!
        self.algorithm = "Ja uz nevim"  # name of your method. Will be used in tournament mode
        self.format_stones()
        self.algorithms = {"Ja uz nevim": self.sub_optimal}
        self.time_start = None
        self.threshold = 0
        self.max_compute_time = 1.1  # in seconds
        self.all_moves_alg = 0
        self.all_moves_choice = {0: self.experimental_neighbours,
                                 1: self.all_legal_moves}

    @staticmethod
    def rotate_stone(stone):
        """
        rotates stone to the left by 90 degrees
        rotation is in place
        """
        for i in range(len(stone)):
            stone[i][0], stone[i][1] = -stone[i][1], stone[i][0]

    @staticmethod
    def figure_out_rotations(stone):
        """
        Finds how many times stone needs to be rotated
        This is achieved by figuring out if its straight piece
                *
        *** or  *  or anything other - straight piece needs to be
                *  rotated only twice other pieces 4 times
        """
        prev_row = None
        prev_col = None
        straight_row = True
        straight_col = True
        for cell in stone:
            if prev_row is None:
                prev_row = cell[0]
            if prev_col is None:
                prev_col = cell[1]

            if prev_row != cell[0]:
                straight_row = False
            if prev_col != cell[1]:
                straight_col = False
            prev_row = cell[0]
            prev_col = cell[1]
        return 2 if (straight_row or straight_col) else 4

    def format_stones(self):
        """
        Formatting stones in place - without changing their order !!
        Every stone is shifted to upper left corner - [0, 0] coord in list
        Every stone will also be rotated 4 times by 90 degrees to the left
        stones are in format [stoneColor, [stoneCells]]
        format will change to [stoneColor, [stoneCells(shifted)], height - rows, width - cols]
        """

        for stone in self.stones:
            cells = stone.pop()
            for _ in range(self.figure_out_rotations(cells)):
                max_row, min_row, max_col, min_col = self.find_max_min(cells)
                height = max_row - min_row + 1
                width = max_col - min_col + 1
                shiftedStones = []
                for cell in cells:
                    shiftedStones.append([cell[0] - min_row, cell[1] - min_col])

                if [shiftedStones, height, width] not in stone:
                    stone.append([shiftedStones, height, width])
                else:
                    break
                self.rotate_stone(cells)

    def neighbours(self, cells, row, col):
        neig = []
        for cell in cells:
            r, c = cell
            tmp = [[r + 1, c], [r - 1, c], [r, c + 1], [r, c - 1]]
            for n in tmp:
                if n not in cells:
                    r_c, c_c = n[0] + row, n[1] + col
                    if -1 < r_c < len(self.board) and -1 < c_c < len(self.board):
                        if [r_c, c_c] not in neig:
                            neig.append([r_c, c_c])
        return neig

    def find_max_min(self, stone):
        """
        Finds biggest and smallest row and col of stone,
        to be later shifted into upper left corner
        format of stone [stoneCells] -> [row, col]
        """
        max_row = max(stone, key=lambda x: x[0])[0]
        min_row = min(stone, key=lambda x: x[0])[0]
        max_col = max(stone, key=lambda x: x[1])[1]
        min_col = min(stone, key=lambda x: x[1])[1]
        return max_row, min_row, max_col, min_col

    def absolute_position(self, cells, row, col, piece_num):
        """
        Constructs absolute position of piece on board, with piece index
        """
        piece = [piece_num]
        positions = []
        for cell in cells:
            positions.append([row + cell[0], col + cell[1]])
        piece.append(positions)
        return piece

    def fit_stone(self, cells, row, col, stone_color, my):
        """
        tries to fit stone starting from row, col position
        if it does not collide with any other piece => successful fit returns True
        else: return False and cell on which it failed to later remove
        part of piece which was tried to be fitted in
        """
        score = 0
        for cell in range(len(cells)):
            if not my:
                score -= self.marks[row + cells[cell][0]][col + cells[cell][1]] * self.player
            else:
                if self.marks[row + cells[cell][0]][col + cells[cell][1]] == -self.player:
                    score += 1
                elif self.marks[row + cells[cell][0]][col + cells[cell][1]] == self.player:
                    score -= 2
            if self.board[row + cells[cell][0]][col + cells[cell][1]] == 0:
                self.board[row + cells[cell][0]][col + cells[cell][1]] = stone_color
            else:
                return False, cell, score
        return True, len(cells), score

    def get_neighbours(self, c_row, c_col, cells, row, col):
        """
        returns up, down, left right neighbours of [row, col]
        """

        temp = [[c_row + 1, c_col], [c_row - 1, c_col], [c_row, c_col + 1], [c_row, c_col - 1]]
        neighbours = []
        for neighbour in temp:
            if -1 < neighbour[0] < len(self.board) and -1 < neighbour[1] < len(self.board[0]):
                if [neighbour[0] - row, neighbour[1] - col] not in cells:
                    if self.board[neighbour[0]][neighbour[1]] != 0:
                        neighbours.append(neighbour)
        return neighbours

    def check_rules(self, cells, row, col):
        """
        after piece was fitted - 3 rules need to be satisfied
        1. at least one edge needs to be in contact with other piece
        2. 2 pieces of same color can not touch
        3. no area of 2x2 cannot be completely filled in
        """
        touched = False
        for cell in cells:
            current_row, current_col = row + cell[0], col + cell[1]
            neighbours = self.get_neighbours(current_row, current_col, cells, row, col)
            # print(cells, "   ", current_row, current_col, "  ", neighbours)
            # CHECKING TOUCHING EDGES
            if len(neighbours) == 0:
                continue
            else:
                for neighbour in neighbours:
                    if self.board[neighbour[0]][neighbour[1]] == self.board[current_row][current_col]:
                        """for line in self.board:
                            print(*line, sep=" ")
                        print(neighbour, current_row, current_col, row, col, self.board[current_row][current_col])
                        """

                        return False
                touched = True
                # CHECK 2x2 spaces
                for i in range(-1, 1):
                    for j in range(-1, 1):
                        # CHECK 4 squares
                        num_filled = 0
                        for k in range(2):
                            for l in range(2):
                                r = current_row + i + k
                                c = current_col + j + l
                                if -1 < r < len(self.board) and -1 < c < len(self.board[0]):
                                    if self.board[r][c] != 0:
                                        num_filled += 1
                        if num_filled == 4:
                            return False

        if not touched:
            return False

        return True

    def clean_up(self, cells, fail_cell, row, col):
        """
        removes any leftover pieces of fitted piece
        """
        for cell in range(fail_cell):
            self.board[row + cells[cell][0]][col + cells[cell][1]] = 0

    def all_legal_moves(self, my=False):
        """
        finds all legal moves and returns their absolute positions
        in a list [[[absolute positions], stone index], ...]
        """
        legal_positions = []
        for stone in range(len(self.stones)):
            # [stoneColor, [], []]
            # if this stone is available - proceed
            if not self.freeStones[stone]:
                continue
            for stoneRotation in range(1, len(self.stones[stone])):
                # [[rotatedCells], height(rows), width(cols), [neighbours] aa ], ...
                for row in range(len(self.board) - self.stones[stone][stoneRotation][1] + 1):
                    for col in range(len(self.board[0]) - self.stones[stone][stoneRotation][2] + 1):
                        # go through every every position on board to (row - pieceHeight), (col - pieceWidth)
                        # try to fit a piece - to not overlap with other piece
                        # if it fits check rules
                        succeed, fail_cell, score = self.fit_stone(self.stones[stone][stoneRotation][0], row, col,
                                                                   self.stones[stone][0], my)
                        if succeed:
                            if self.check_rules(self.stones[stone][stoneRotation][0], row, col):
                                # make absolute position of piece
                                # maybe remove the fitted piece - not now
                                # appends position as - stone, stone rotation, row, col, score - my score
                                legal_positions.append([stone, stoneRotation, row, col, score])
                            # clean up piece that did not satisfy rules
                            # or clean up good piece to make room for new one
                            self.clean_up(self.stones[stone][stoneRotation][0], fail_cell, row, col)
                        else:
                            # remove any leftovers from this piece
                            self.clean_up(self.stones[stone][stoneRotation][0], fail_cell, row, col)
        return legal_positions

    def two_two_rule(self, row_n, col_n, row, col, cells):
        d_x, d_y = row_n - row, col_n - col
        for i in range(-1, 1):
            for j in range(-1, 1):
                num_filled = 0
                for k in range(2):
                    for l in range(2):
                        if [d_x + i + k, d_y + j + l] in cells:
                            num_filled += 1
                            continue
                        r = row_n + i + k
                        c = col_n + j + l
                        if -1 < r < len(self.board) and -1 < c < len(self.board):
                            if self.board[r][c] != 0:
                                num_filled += 1
                if num_filled > 2:
                    return True
        return False

    def first_move(self):
        # kinda same as all legal moves - except it does not check
        # game rules cuz first stone can be placed anywhere
        # finds the move which is adds me the most score
        best_score = -float("inf")
        best_move_index = []  # best move index and row, col start position
        for stone in range(len(self.stones)):
            for stoneRot in range(1, len(self.stones[stone])):
                for row in range(len(self.board) - self.stones[stone][stoneRot][1] + 1):
                    for col in range(len(self.board[0]) - self.stones[stone][stoneRot][2] + 1):
                        enemy_score = 0
                        my_score = 0
                        n_b = self.neighbours(self.stones[stone][stoneRot][0], row, col)
                        for n in n_b:
                            result = self.two_two_rule(n[0], n[1], row, col, self.stones[stone][stoneRot][0])
                            if result:
                                if self.marks[n[0]][n[1]] == self.player:
                                    #print("Defended my piece")
                                    my_score += 1
                                elif self.marks[n[0]][n[1]] == -self.player:
                                    #print("defended enemy piece")
                                    enemy_score += 1
                        for cell in self.stones[stone][stoneRot][0]:
                            if self.marks[row + cell[0]][col + cell[1]] == -self.player:
                                my_score += 1
                            elif self.marks[row + cell[0]][col + cell[1]] == self.player:
                                enemy_score += 1
                        if (my_score - enemy_score) > best_score:
                            best_score = (my_score - enemy_score)
                            best_move_index = [stone, stoneRot, row, col]
            if time.time() - self.time_start > self.max_compute_time:
                break

        return self.absolute_position(self.stones[best_move_index[0]][best_move_index[1]][0], best_move_index[2],
                                      best_move_index[3], best_move_index[0])

    def enemy_score(self, stone, stone_rot, row, col):
        """
        Counts enemy score
        1. place my stone
        2. get all enemy positions
        3. get best enemy score
        4. remove my stone
        """
        # Place my stone
        for cell in self.stones[stone][stone_rot][0]:
            self.board[row + cell[0]][col + cell[1]] = self.stones[stone][0]

        self.freeStones[stone] = False

        # Get all enemy moves
        # enemy_moves = self.all_legal_moves()
        enemy_moves = self.all_moves_choice[self.all_moves_alg]()
        # Remove my stone
        for cell in self.stones[stone][stone_rot][0]:
            self.board[row + cell[0]][col + cell[1]] = 0
        self.freeStones[stone] = True

        if len(enemy_moves) == 0:
            return 0
        # counts the best enemy score
        # legal move is [stone, stoneRot, row, col, score]
        # score is my score so the smallest is best enemy score
        best_enemy_score = None
        for enemy_move in enemy_moves:
            if best_enemy_score is None or enemy_move[4] < best_enemy_score:
                best_enemy_score = enemy_move[4]
        best_enemy_score = best_enemy_score * (-1)
        return best_enemy_score

    def experimental_neighbours(self, my=False):
        """
        find extremes on board  list each index is row and inside is leftmost and rightmost index[[L, R], [L, R]
        """
        up = None
        down = 0
        left = None
        right = 0
        for row in range(len(self.board)):
            empty = True
            for col in range(len(self.board)):
                if self.board[row][col] != 0:
                    empty = False
                    if left is None or col < left:
                        left = col
                    if col > right:
                        right = col
            if not empty:
                if up is None:
                    up = row
                if row > down:
                    down = row

        if up == 0 and down == len(self.board) and left == 0 and right == len(self.board):
            self.all_moves_alg = 1
        legal_positions = []

        for stone in range(len(self.stones)):
            # [stoneColor, [], []]
            # if this stone is available - proceed
            if not self.freeStones[stone]:
                continue
            for stoneRotation in range(1, len(self.stones[stone])):
                # [[rotatedCells], height(rows), width(cols)], ...
                start = max(up - self.stones[stone][stoneRotation][1], 0)
                end = min(down + 2, len(self.board) - self.stones[stone][stoneRotation][1] + 1)
                for row in range(start, end):
                    s = max(left - self.stones[stone][stoneRotation][2], 0)
                    e = min(right + 2, len(self.board[0]) - self.stones[stone][stoneRotation][2] + 1)
                    for col in range(s, e):
                        succeed, fail_cell, score = self.fit_stone(self.stones[stone][stoneRotation][0], row, col,
                                                                   self.stones[stone][0], my)
                        if succeed:
                            if self.check_rules(self.stones[stone][stoneRotation][0], row, col):
                                # make absolute position of piece
                                # maybe remove the fitted piece - not now
                                # appends position as - stone, stone rotation, row, col, score - my score
                                legal_positions.append([stone, stoneRotation, row, col, score])
                            # clean up piece that did not satisfy rules
                            # or clean up good piece to make room for new one
                            self.clean_up(self.stones[stone][stoneRotation][0], fail_cell, row, col)
                        else:
                            # remove any leftovers from this piece
                            self.clean_up(self.stones[stone][stoneRotation][0], fail_cell, row, col)
        return legal_positions

    def sub_optimal(self):
        """
        Takes all my possible moves, orders them by how much score gain i get
        start with the best one and looks at all of enemy possible moves, if
        the best enemy score makes bigger score diff - new best score - continue till
        i have time
        """
        if self.isEmpty():
            # print("first move on empty board")
            return self.first_move()

        legal_moves = self.all_moves_choice[self.all_moves_alg](True)

        if len(legal_moves) == 0:
            return []
        legal_moves = sorted(legal_moves, key=lambda x: x[4], reverse=True)
        print(legal_moves[0])
        #count = 0
        best_move = None
        best_score = None
        best_move_def = 0
        for legal_move in legal_moves:
            defended = 0
            # score is my score
            score = legal_move.pop()
            n_b = self.neighbours(self.stones[legal_move[0]][legal_move[1]][0], legal_move[2], legal_move[3])
            for n in n_b:
                if self.board[n[0]][n[1]] != 0:
                    continue
                result = self.two_two_rule(n[0], n[1], legal_move[2], legal_move[3], self.stones[legal_move[0]][legal_move[1]][0])
                if result:
                    if self.marks[n[0]][n[1]] == self.player:
                        defended += 1
                        score += 1
                    elif self.marks[n[0]][n[1]] == -self.player:
                        defended += 1
                        score -= 1

            # enemy_score is max score enemy can get with best move
            enemy_score = self.enemy_score(*legal_move)
            if best_move is None:
                print(score, defended, enemy_score)

            score_diff = score - enemy_score

            if best_score is None or score_diff > best_score:
                best_score = score_diff
                best_move = legal_move
                best_move_def = defended
            if time.time() - self.time_start > self.max_compute_time:
                print("defended - ", best_move_def, best_score)
                #print("done ", count, " out of ", len(legal_moves))
                return self.absolute_position(self.stones[best_move[0]][best_move[1]][0], best_move[2], best_move[3],
                                              best_move[0])

            #count += 1
        print("defended - ", best_move_def, best_score)
        return self.absolute_position(self.stones[best_move[0]][best_move[1]][0], best_move[2], best_move[3],
                                      best_move[0])

    def move(self):
        """ return [ stoneIdx, [ stonePosition] ]
            stoneIdx .. integer .. index of stone to self.freeStones
            [stonePosition] = [ [row1,col1] ... [rown, coln] ] .. position into board where stone is placed

            if no stone can be placed:
            return []
        """
        self.time_start = time.time()
        move = self.algorithms[self.algorithm]()
        return move


if __name__ == "__main__":
    # load stones from file
    stones = base.loadStones("stones.txt")
    # prepare board and marks
    board, marks = base.makeBoard10()
    # create both players
    p1 = Player("pepa", board, marks, stones, 1)
    p2 = Player("franta", board, marks, stones, -1)
    """for stone in p1.stones:
        print(stone)
    """
    # not necessary, only if you want to draw board to png files
    d = Drawer()
    d.draw(p1.board, p1.marks, "init.png")

    moveidx = 0
    while True:
        p1play = True
        p2play = True
        time1 = time.time()
        # m = profile.runcall(p1.move)  # first player, we assume that a corrent output is returned
        print("Player A - smart")
        m = p1.move()
        print(round((time.time() - time1) * 1000, 3), "ms")
        # stats = pstats.Stats(profile)
        # stats.print_stats()
        # the following if/else is simplified. On Brute, we will check if return value
        # from move() is valid ...
        if len(m) == 0:
            p1play = False
        else:
            stoneIdx, stone = m
            print(stoneIdx, " stone number")
            stoneColor = stones[stoneIdx][0]
            base.writeBoard(p1.board, stone, stoneColor)  # write stone to player1's board
            base.writeBoard(p2.board, stone, stoneColor)  # write stone to player2's board
            p1.freeStones[stoneIdx] = False  # tell player1 which stone is used
            p2.freeStones[stoneIdx] = False  # tell player2 which stone is used
        d.draw(p2.board, p2.marks, "move-{:02d}a.png".format(moveidx))  # draw to png
        """for line in p1.board:
            print(*line, sep=" ")"""
        # now we call player2 and update boards/freeStones of both players
        time1 = time.time()
        # profile = cProfile.Profile()
        # m = profile.runcall(p2.move)
        print("\n")
        print("player B - stupid")
        m = p2.move()
        # stats = pstats.Stats(profile)
        # stats.print_stats()

        print(round((time.time() - time1) * 1000, 3), "ms")
        if len(m) == 0:
            p2play = False
        else:
            stoneIdx, stone = m
            print(stoneIdx, "stone number")
            stoneColor = stones[stoneIdx][0]
            base.writeBoard(p1.board, stone, stoneColor)
            base.writeBoard(p2.board, stone, stoneColor)
            p1.freeStones[stoneIdx] = False
            p2.freeStones[stoneIdx] = False
        d.draw(p1.board, p1.marks, "move-{:02d}b.png".format(moveidx))

        # if both players return [] from move, the game ends
        if p1play == False and p2play == False:
            print("end of game")
            print(moveidx, " - moves")
            break

        moveidx += 1
        print(" -- end of move ", moveidx, " score is ", p1.score(p1.player), p1.score(-p1.player))
        print("\n")