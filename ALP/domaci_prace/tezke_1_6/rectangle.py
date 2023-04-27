import sys


def find_largest_area(hist):
    stack = []
    max_area = 0
    i = 0
    left_index = 0
    right_index = 0
    height = 0
    while i < len(hist):

        if (not stack) or (hist[stack[-1]] <= hist[i]):
            stack.append(i)
            i += 1

        else:

            stack_top = stack.pop()

            area = (hist[stack_top] * ((i - stack[-1] - 1) if stack else i))

            if area > max_area:
                max_area = area
                height = hist[stack_top]
                left_index = int(i - (max_area / hist[stack_top]))
                right_index = i - 1

    while stack:
        stack_top = stack.pop()

        area = (hist[stack_top] * ((i - stack[-1] - 1) if stack else i))

        if area > max_area:
            max_area = area
            height = hist[stack_top]
            left_index = int(i - (max_area / hist[stack_top]))
            right_index = i - 1
    return max_area, left_index, right_index, height


def load_matrix(path):
    matrix = []
    with open(path, "r") as f:
        for line in f.readlines():
            matrix.append(list(map(int, line.split())))
    f.close()

    histogram = [0] * len(matrix[0])

    return matrix, histogram


def update_histogram(matrix, row, histogram):
    for i in range(len(matrix[row])):
        if matrix[row][i] < 0:
            histogram[i] += 1
        else:
            histogram[i] = 0


def main():
    path = sys.argv[1]
    matrix, histogram = load_matrix(path)
    top_left = [0, 0]
    down_right = [-1, -1]
    max_area = 0

    for row in range(len(matrix)):
        update_histogram(matrix, row, histogram)
        area, left_index, right_index, height = find_largest_area(histogram)
        if area > max_area:
            max_area = area
            top_left = [left_index, row - height + 1]
            down_right = [right_index, row]

    print(top_left[1], top_left[0])
    print(down_right[1], down_right[0])


if __name__ == "__main__":
    main()
