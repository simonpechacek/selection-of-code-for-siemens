"""
Adam7 algorithm
input:
R | C - number of rows | number of cols
[] - raw data - can be only a fraction of needed data
Output:
constructed interpolated "image"
"""

"""
Block 1 - 6 have indexes, Block 7 - has rows that needs to be filled in
"""
block_indexes = {
    1: [[[0, 0]]],
    2: [[[0, 4]]],
    3: [[[4, 0], [4, 4]]],
    4: [[[0, 2], [0, 6]], [[4, 2], [4, 6]]],
    5: [[[2, 0], [2, 2], [2, 4], [2, 6]], [[6, 0], [6, 2], [6, 4], [6, 6]]],
    6: [[[0, 1], [0, 3], [0, 5], [0, 7]], [[2, 1], [2, 3], [2, 5], [2, 7]], [[4, 1], [4, 3], [4, 5], [4, 7]], [[6, 1], [6, 3],
        [6, 5], [6, 7]]],
    7: [[1], [3], [5], [7]]
}


def load_input():
    cols, rows = list(map(int, input().split()))
    raw_data = list(map(int, input().split()))
    return rows, cols, raw_data


def load_input_test():
    with open("test.txt", "r") as f:
        cols, rows = list(map(int, f.readline().split()))
        raw_data = list(map(int, f.readline().split()))
    f.close()
    return rows, cols, raw_data


def print_raw_img(img):
    for b_row in img:
        for r in range(8):
            for j in range(len(b_row)):
                for i in range(8):
                    if b_row[j][r][i] < 0:
                        print("#", end="")
                        continue
                    print(b_row[j][r][i], end="")
                print("|", end="")
            print()
        print("-" * (len(b_row) * 8 * 2 + 3))


def print_img(img):
    for b_row in img:
        for r in range(8):
            for j in range(len(b_row)):
                for i in range(8):
                    if b_row[j][r][i] in [0, 2]:
                        print(" ", end="")
                    else:
                        print("*", end="")
            print()


def create_8blocks(rows, cols):
    """
    creates 2D list of sub 8x8 2D blocks
    [
       [ [ []      [ []
           []        []
           [] ] ,    [] ], --- ],


    ]
    """
    r = rows // 8
    c = cols // 8
    num8blocks = r * c
    img = []
    for row in range(r):
        row_list = []
        for col in range(c):
            row_list.append([[-1 for _ in range(8)] for _ in range(8)])
        img.append(row_list)

    # print(r, c)
    # print(img)

    return img, r, c


def fill_img(image, pass_num, remaining_nums, r_blocks, c_blocks):
    """
    index => points to num in remaining nums which is next to be filled in
    """
    #print(pass_num, remaining_nums)
    if len(remaining_nums) == 0:
        return
    index = 0
    for row in range(r_blocks):
        for sub_row in block_indexes[pass_num]:
            for col in range(c_blocks):
                if pass_num != 7:
                    for block in sub_row:
                        image[row][col][block[0]][block[1]] = remaining_nums[index]
                        index += 1
                else:
                    for r_7 in sub_row:
                        for j in range(8):
                            image[row][col][r_7][j] = remaining_nums[index]
                            index += 1
    #print(index)
    #quit()
    fill_img(image, pass_num + 1, remaining_nums[index:], r_blocks, c_blocks)


def find_nearest(c_block, i, j):
    """
    i is row, j is col
    c_block is current block that os worked on
    """
    j_start = j
    while j > 0:
        j -= 1
        if c_block[i][j] in [0, 1]:
            return 2 if c_block[i][j] == 0 else 3
        else:
            continue
    while i > 0:
        i -= 1
        if c_block[i][j_start] == -1:
            continue
        else:
            return 2 if c_block[i][j_start] in [0, 2] else 3


def fill_in_the_blanks(image):
    """
    replaces -1 with nearest defined value on the left
    when there is nothing on the left - replaces with nearest
    non -1 value above

    """
    for r_block in image:
        for c_block in r_block:
            for i in range(8):
                for j in range(8):
                    if c_block[i][j] == -1:
                        c_block[i][j] = find_nearest(c_block, i, j)



def main():
    """
    1 - 1
    2 - 1
    3 - 2
    4 - 4
    5 - 8
    .
    7 - 32
    numbers needed => number of occurrences in 8x8 grid * "num of 8x8 grids"
    at least "num 8x8 grids" of numbers is needed - first "num 8x8 grids" numbers are always 1

    """
    rows, cols, raw_data = load_input()
    # print(rows, cols)
    # print(raw_data)

    image, row_blocks, col_blocks = create_8blocks(rows, cols)
    #print(row_blocks, col_blocks)
    fill_img(image, 1, raw_data, row_blocks, col_blocks)
    #print_raw_img(image)
    fill_in_the_blanks(image)
    print_img(image)


if __name__ == "__main__":
    main()
