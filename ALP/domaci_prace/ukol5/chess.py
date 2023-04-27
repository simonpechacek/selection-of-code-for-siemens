"""
a8 = 0, 0 => 0. řádek 0. sloupec
černí jsou "nahoře" pole

hledám tah bílého, který dá šach černému

+1 - bílý
-1 - černý

1 - král - king - 1 - pole kolem sebe
2 - dáma - queen - do všech směrů
3 - věž - rook - jenom horizontálně/vertikálně
4 - střelec - bishop - diagonálně
5 - jezdec - knight - L
6 - pěšec - pawn - pouze 1 pole do předu - nemůžeme ho změnit na jinou figurku - neuvažujeme en passant
0 - volné pole

pole je 8x8

queen, rook a bishop mají místo souřadnic směr kterým můžou "chodit"

"""
import sys

pieces = {
    1: [],
    2: [],
    3: [[1, 0], [0, 1], [-1, 0], [0, -1]],  # this is direction
    4: [[1, 1], [-1, -1], [1, -1], [-1, 1]],  # this is direction
    5: [[-2, -1], [-2, 1], [2, -1], [2, 1], [1, 2], [-1, 2], [1, -2], [-1, -2]],  # these are coord
    6: [[-1, 0], [-1, -1], [-1, 1]]
}


def king_moves():
    for i in range(-1, 2):
        for j in range(-1, 2):
            if i == 0 and j == 0:
                continue
            pieces[1].append([i, j])


king_moves()
pieces[2] = pieces[3] + pieces[4]


def find_kings(chess_board):
    black_king = []
    white_king = []
    for r, row in enumerate(chess_board):
        for c, piece in enumerate(row):
            if piece == 1:
                white_king = [r, c]
            elif piece == -1:
                black_king = [r, c]
    return white_king, black_king


def unmovable_pieces(chess_board, white_king):
    unmov_pieces = []
    for r, row in enumerate(chess_board):
        for c, piece in enumerate(row):
            if piece in [-2, -3, -4]:
                for dir in pieces[abs(piece)]:
                    white_pieces = []
                    attacking_king = False
                    row_ = r + dir[0]
                    column = c + dir[1]
                    while 0 <= row_ < 8 and 0 <= column < 8:
                        if [row_, column] == white_king:
                            attacking_king = True
                            break

                        if chess_board[row_][column] > 0:
                            white_pieces.append([row_, column])
                        row_ += dir[0]
                        column += dir[1]

                    if attacking_king and len(white_pieces) == 1:
                        unmov_pieces.append(white_pieces[0])
                        break
    return unmov_pieces


def pawn_move(chess_board, black_king, current_pos):
    possible_moves = []
    check = None
    for num, move in enumerate(pieces[6]):
        row = current_pos[0] + move[0]
        column = current_pos[1] + move[1]
        if 0<=row < 8 and 0<= column< 8:
            if num == 0:
                if chess_board[row][column] == 0:
                    possible_moves.append([row, column])
            elif num > 0:
                if chess_board[row][column] < 0:
                    possible_moves.append([row, column])

    for possible_move in possible_moves:
        for move in [[-1, 1], [-1, -1]]:
            row = possible_move[0] + move[0]
            column = possible_move[1] + move[1]
            if 0 <= row < 8 and 0 <= column < 8:
                if [row, column] == black_king:
                    check = possible_move
                    return current_pos, check

    return current_pos, check


def knight_move(chess_board, black_king, current_pos):
    possible_moves = []
    check = None
    for move in pieces[5]:
        row = current_pos[0] + move[0]
        column = current_pos[1] + move[1]
        if 0 <= row < 8 and 0 <= column < 8:
            if chess_board[row][column] <= 0:
                possible_moves.append([row, column])
    for p_move in possible_moves:
        for move in pieces[5]:
            row = p_move[0] + move[0]
            column = p_move[1] + move[1]
            if 0 <= row < 8 and 0 <= column < 8:
                if [row, column] == black_king:
                    check = p_move
                    return current_pos, check
    return current_pos, check


def directional_pieces(chess_board, black_king, current_pos, chess_piece):
    possible_moves = []
    check_move = None
    for dir in pieces[chess_piece]:
        row = current_pos[0] + dir[0]
        column = current_pos[1] + dir[1]
        while 0 <= row < 8 and 0 <= column < 8:
            if chess_board[row][column] != 0:
                if chess_board[row][column] < 0:
                    possible_moves.append([row, column])
                break
            possible_moves.append([row, column])
            row += dir[0]
            column += dir[1]
    for start_pos in possible_moves:
        for dir in pieces[chess_piece]:
            row = start_pos[0] + dir[0]
            column = start_pos[1] + dir[1]
            while 0 <= row < 8 and 0 <= column < 8:
                if chess_board[row][column] != 0 and chess_board[row][column] != -1:
                    break
                if [row, column] == black_king:
                    check_move = start_pos
                    return current_pos, check_move
                row += dir[0]
                column += dir[1]
    return current_pos, check_move


def main():
    file_path = sys.argv[1]
    chess_board = []
    with open(file_path, "r") as f:
        for line in f:
            chess_board.append(list(map(int, line.split())))

        f.close()

    white_k, black_k = find_kings(chess_board)
    unmov_pieces = unmovable_pieces(chess_board, white_k)

    for r, row in enumerate(chess_board):
        for c, piece in enumerate(row):
            if piece in [2, 3, 4] and [r, c] not in unmov_pieces:
                start, check = directional_pieces(chess_board, black_k, [r, c], piece)
                if check is not None:
                    print(start[0], start[1], check[0], check[1])
                    return
            elif piece == 5 and [r, c] not in unmov_pieces:
                start, check = knight_move(chess_board, black_k, [r, c])
                if check is not None:
                    print(start[0], start[1], check[0], check[1])
                    return
            elif piece == 6 and [r, c] not in unmov_pieces:
                start, check = pawn_move(chess_board, black_k, [r, c])
                if check is not None:
                    print(start[0], start[1], check[0], check[1])
                    return


if __name__ == "__main__":
    main()
