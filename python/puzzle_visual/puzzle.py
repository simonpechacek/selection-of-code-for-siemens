"""
This program fills  M x N size matrix with given stones
It returns only the first found solution
Input is text file containing info about num of rows, cols and stones
"""
import sys
import time

import pygame
import random
import threading

pygame.init()


def readStones(filename):
    _stones = []  # vystup funkce - seznam nactenych kamenu
    f = open(filename, "r")
    numRows = int(f.readline().strip())  # precti 1. radek ze souboru
    numCols = int(f.readline().strip())  # precti 2. radek ze souboru
    for line in f:
        numbers = list(map(int, line.strip().split()))  # precti vsechna cisla na dalsich radcich
        color = numbers[0]  # prvni z nich je barva kamene
        coords = numbers[1:]  # zbyle jsou souradnice r1 c1 ... rn cn
        cells = []  # prevedeme [r1,c1 ... rn,cn] na pole cells = [[r1,c1], ... [rn,cn]]
        for i in range(len(coords) // 2):
            cells.append([coords[2 * i + 0], coords[2 * i + 1]])
        _stones.append([color, cells])
    f.close()
    return numRows, numCols, _stones


def getMin(stone):
    min_r = None
    max_r = None
    min_c = None
    max_c = None
    for coord in (stone[1]):
        if min_r is None or coord[0] < min_r:
            min_r = coord[0]
        if max_r is None or coord[0] > max_r:
            max_r = coord[0]
        if min_c is None or coord[1] < min_c:
            min_c = coord[1]
        if max_c is None or coord[1] > max_c:
            max_c = coord[1]
    return min_r, min_c, (max_r - min_r) + 1, (max_c - min_c) + 1


def adjustStone(stone, x, y):
    for coord in stone[1]:
        coord[0] -= x
        coord[1] -= y


def createStone(stone, dr, dc):
    newStone = [[0 for _ in range(dc)] for _ in range(dr)]
    for coord in stone[1]:
        row, col = coord[0], coord[1]
        newStone[row][col] = stone[0]
    return newStone


def prettyStones(stones):
    for stone in stones:
        # min_r lowest row of stone
        # min_c lowest column of stone
        # dr height in rows of stone
        # dc width in cols of stone
        min_r, min_c, dr, dc = getMin(stone)
        adjustStone(stone, min_r, min_c)
        stone[1] = createStone(stone, dr, dc)


def print_stones(stones):
    for stone in stones:
        for st in stone[1]:
            print(*st, sep=" ")


def fitStone(stone, row, col, board):
    for i in range(len(stone[1])):
        # i - current row
        for j in range(len(stone[1][0])):
            # j - current col
            if (row + i > len(board) - 1) or (col + j > len(board[0]) - 1):
                return False
            if board[row + i][col + j] == 0:
                board[row + i][col + j] = stone[1][i][j]
            elif stone[1][i][j] == 0:
                continue
            else:
                return False
    return True


def stoneArea(stone):
    if type(stone[1][0]) == list:
        return len(stone[1]) * len(stone[1][0])
    else:
        return 1


def arrangeStones(stones):
    stones.sort(key=stoneArea, reverse=True)


def cleanUp(board, row, col, stone):
    color = stone[0]
    height = len(board) - 1
    width = len(board[0]) - 1

    for i in range(len(stone[1])):
        for j in range(len(stone[1][0])):
            if row + i > height or col + j > width:
                return
            if board[row + i][col + j] == color:
                board[row + i][col + j] = 0


def print_board(board):
    for line in board:
        print(*line, sep=" ")
    print()


def fill(board, stoneNum, stones):
    # print_board(board)
    for row in range(len(board)):
        for col in range(len(board[0])):
            succeeded = fitStone(stones[stoneNum], row, col, board)
            if succeeded:
                draw(board)
                if run[1]:
                    run[0] = False
                    while not run[0]:
                        continue
                if stoneNum + 1 == len(stones):
                    print("ending")
                    return True
                else:
                    if fill(board, stoneNum + 1, stones):
                        return True
                    cleanUp(board, row, col, stones[stoneNum])
            else:
                cleanUp(board, row, col, stones[stoneNum])
    if stoneNum == 0:
        print("NOSOLUTION")
    return False


def draw(board):
    win.fill((100, 100, 100))
    for row in range(len(board)):
        for col in range(len(board[0])):
            pygame.draw.rect(win, colors[board[row][col]], (col * tile_size, row * tile_size, tile_size, tile_size))
            pygame.draw.line(win, "black", (tile_size * col, 0), (tile_size * col, M * tile_size), 3)
        pygame.draw.line(win, "black", (0, row * tile_size), (N * tile_size, row * tile_size), 3)
    pygame.display.update()


M, N, stones = readStones("puzzle.txt")
tile_size = 80
win = pygame.display.set_mode((N * tile_size, M * tile_size))
colors = {
    0: (100, 100, 100),
    1: "#55efc4",
    2: "#74b9ff",
    3: "#a29bfe",
    4: "#00b894",
    5: "#fd79a8",
    6: "#d63031",
    7: "#ffeaa7",
    8: "#0984e3"
}
run = [True, True]


def main():
    prettyStones(stones)
    arrangeStones(stones)

    # print_stones(stones)
    board = [[0 for _ in range(N)] for _ in range(M)]
    running = True
    clock = pygame.time.Clock()
    threading.Thread(target=fill, args=(board, 0, stones)).start()

    while running:
        clock.tick(2)
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                run[1] = False
                running = False
        run[0] = True


if __name__ == "__main__":
    main()
