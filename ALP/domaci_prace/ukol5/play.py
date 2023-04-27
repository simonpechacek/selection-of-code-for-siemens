import sys

"""
0 = volné pole
1 = křížek
2 = kolečko

hledám - kam táhne křížek

"""

file_path = sys.argv[1]
game_list = []

with open(file_path, "r") as f:
    for line in f.readlines():
        game_list.append(list(map(int, line.strip().split())))
    f.close()

n = len(game_list)

dir_x = [-1, 0, 1, 1]
dir_y = [1, 1, 1, 0]


def check_pos(matrix, row, column, in_row, spaces):
    if matrix[row][column] == 1:
        in_row += 1
    elif matrix[row][column] == 2:
        in_row = 0
        spaces = [0, None]
    elif matrix[row][column] == 0:
        if spaces[0] == 0 and in_row > 0:
            spaces[0] += 1
            spaces[1] = (row, column)
        elif spaces[0] > 0:
            spaces = [0, None]
            in_row = 0

    return in_row, spaces


def find_space(matrix, n, row, column):
    if 0 <= row < n and 0 <= column < n:
        if matrix[row][column] == 0:
            return row, column


def check_straight(matrix, n):
    for i in range(n):
        horizontal = 0
        horizontal_spaces = [0, None]
        vertical = 0
        vertical_spaces = [0, None]
        for j in range(n):
            """ HORIZONTAL CHECKS"""
            horizontal, horizontal_spaces = check_pos(matrix, i, j, horizontal, horizontal_spaces)
            if horizontal == 4 and horizontal_spaces[1] is not None:
                break
            elif horizontal == 4:
                left = j - 4
                right = j + 1
                horizontal_spaces[1] = find_space(matrix, n, i, left)
                if horizontal_spaces[1]:
                    break
                horizontal_spaces[1] = find_space(matrix, n, i, right)
                if horizontal_spaces[1]:
                    break

            elif (j == n - 1) and (horizontal < 4 or horizontal_spaces[0] == 0):
                horizontal_spaces[1] = None
            """ VERTICAL CHECKS"""
            vertical, vertical_spaces = check_pos(matrix, j, i, vertical, vertical_spaces)
            if vertical == 4 and vertical_spaces[1] is not None:
                horizontal_spaces = [0, None]
                break

            elif vertical == 4:
                up = j - 4
                down = j + 1
                vertical_spaces[1] = find_space(matrix, n, up, i)
                if vertical_spaces[1]:
                    horizontal_spaces = [0, None]
                    break
                vertical_spaces[1] = find_space(matrix, n, down, i)
                if vertical_spaces[1]:
                    horizontal_spaces = [0, None]
                    break
            elif (j == n - 1) and (vertical < 4 or vertical_spaces[0] == 0):
                vertical_spaces[1] = None

        if horizontal_spaces[1] is not None:
            return horizontal_spaces[1]

        if vertical_spaces[1] is not None:
            return vertical_spaces[1]


def check_diagonals(matrix, n):
    for i in range(n - 4):
        diag_up_r = 0
        diag_down_r = 0
        diag_up_spaces_r = [0, None]
        diag_down_spaces_r = [0, None]

        diag_up_l = 0
        diag_down_l = 0
        diag_up_spaces_l = [0, None]
        diag_down_spaces_l = [0, None]
        for j in range(n - i):
            diag_up_r, diag_up_spaces_r = check_pos(matrix, j, i + j, diag_up_r, diag_up_spaces_r)
            if diag_up_r == 4 and diag_up_spaces_r[1] is not None:
                diag_up_spaces_l = [0, None]
                diag_down_spaces_r = [0, None]
                diag_down_spaces_l = [0, None]
                break
            elif diag_up_r == 4:
                row, col = j - 4, i + j - 4
                diag_up_spaces_r[1] = find_space(matrix, n, row, col)
                if diag_up_spaces_r[1]:
                    diag_up_spaces_l = [0, None]
                    diag_down_spaces_r = [0, None]
                    diag_down_spaces_l = [0, None]
                    break
                diag_up_spaces_r[1] = find_space(matrix, n, j + 1, i + j + 1)
                if diag_up_spaces_r[1]:
                    diag_up_spaces_l = [0, None]
                    diag_down_spaces_r = [0, None]
                    diag_down_spaces_l = [0, None]
                    break
            elif (j == n - 1 - i) and (diag_up_r < 4 or diag_up_spaces_r[0] == 0):
                diag_up_spaces_r[1] = None

            diag_up_l, diag_up_spaces_l = check_pos(matrix, j, n - 1 - i - j, diag_up_l, diag_up_spaces_l)
            if diag_up_l == 4 and diag_up_spaces_l[1] is not None:
                diag_up_spaces_r = [0, None]
                diag_down_spaces_r = [0, None]
                diag_down_spaces_l = [0, None]
                break
            elif diag_up_l == 4:
                row, col = j - 4, n - 1 - i - j + 4
                diag_up_spaces_l[1] = find_space(matrix, n, row, col)
                if diag_up_spaces_l[1]:
                    diag_up_spaces_r = [0, None]
                    diag_down_spaces_r = [0, None]
                    diag_down_spaces_l = [0, None]
                    break
                diag_up_spaces_l[1] = find_space(matrix, n, j + 1, n - 2 - i - j)
                if diag_up_spaces_l[1]:
                    diag_up_spaces_r = [0, None]
                    diag_down_spaces_r = [0, None]
                    diag_down_spaces_l = [0, None]
                    break
            elif (j == n - 1 - i) and (diag_up_l < 4 or diag_up_spaces_l[0] == 0):
                diag_up_spaces_l[1] = None

            if i == 0:
                continue

            diag_down_r, diag_down_spaces_r = check_pos(matrix, i + j, j, diag_down_r, diag_down_spaces_r)
            if diag_down_r == 4 and diag_down_spaces_r[1] is not None:
                diag_up_spaces_r = [0, None]
                diag_up_spaces_l = [0, None]
                diag_down_spaces_l = [0, None]
                break
            elif diag_down_r == 4:
                row, col = i + j - 4, j - 4
                diag_down_spaces_r[1] = find_space(matrix, n, row, col)
                if diag_down_spaces_r[1]:
                    diag_up_spaces_r = [0, None]
                    diag_up_spaces_l = [0, None]
                    diag_down_spaces_l = [0, None]
                    break
                diag_down_spaces_r[1] = find_space(matrix, n, i + j + 1, j + 1)
                if diag_down_spaces_r[1]:
                    diag_up_spaces_r = [0, None]
                    diag_up_spaces_l = [0, None]
                    diag_down_spaces_l = [0, None]
                    break
            elif (j == n - 1 - i) and (diag_down_r < 4 or diag_down_spaces_r[0] == 0):
                diag_down_spaces_r[1] = None

            diag_down_l, diag_down_spaces_l = check_pos(matrix, i + j, n - 1 - j, diag_down_l, diag_down_spaces_l)
            if diag_down_l == 4 and diag_down_spaces_l[1] is not None:
                diag_up_spaces_r = [0, None]
                diag_up_spaces_l = [0, None]
                diag_down_spaces_r = [0, None]
                break
            elif diag_down_l == 4:
                row, col = i + j - 4, n - 1 - j + 4
                diag_down_spaces_l[1] = find_space(matrix, n, row, col)
                if diag_down_spaces_l[1]:
                    diag_up_spaces_r = [0, None]
                    diag_up_spaces_l = [0, None]
                    diag_down_spaces_r = [0, None]
                    break
                diag_down_spaces_l[1] = find_space(matrix, n, i + j - 1, n - 1 - j + 1)
                if diag_down_spaces_l[1]:
                    diag_up_spaces_r = [0, None]
                    diag_up_spaces_l = [0, None]
                    diag_down_spaces_r = [0, None]
                    break
            elif (j == n - 1 - i) and (diag_down_l < 4 or diag_down_spaces_l[0] == 0):
                diag_down_spaces_l[1] = None

        if diag_up_spaces_r[1] is not None:
            return diag_up_spaces_r[1]

        if diag_up_spaces_l[1] is not None:
            return diag_up_spaces_l[1]

        if diag_down_spaces_r[1] is not None:
            return diag_down_spaces_r[1]

        if diag_down_spaces_l[1] is not None:
            return diag_down_spaces_l[1]


pos = check_straight(game_list, n)
if pos:
    print(pos[0], pos[1])
else:
    pos = check_diagonals(game_list, n)
    if pos:
        print(pos[0], pos[1])
