import sys

equations = []

for line in sys.stdin:
    equations.append(line.strip())


def pm(a):
    for r in range(len(a)):
        for s in range(len(a[0])):
            print(a[r][s], end=" ")
        print()


def get_all_letters(equat):
    all_letters = []
    for row in equat:
        for letter in row:
            if (97 <= ord(letter) <= 122) and letter not in all_letters:
                all_letters.append(letter)
    return sorted(all_letters)


def format_equations(equat):
    formatted = []
    for equation in equat:
        temp_line = equation.split("=")
        left = [i for i in temp_line[0].split("+")]
        right = [j for j in temp_line[1].split("+")]
        formatted.append([left, right])
    return formatted


def get_letter_index(letter, sorted_letters):
    for let in range(len(sorted_letters)):
        if letter == sorted_letters[let]:
            return let


def make_matrix(equation, sorted_letters):
    rows = len(equation)
    cols = len(sorted_letters)
    matrix = []

    for r in range(rows):
        row = [0] * (cols + 1)
        for j in range(2):
            for l in range(len(equation[r][j])):
                if len(equation[r][j][l]) > 1:
                    num = int(equation[r][j][l][:-1])
                    letter = equation[r][j][l][-1]
                else:
                    letter = equation[r][j][l]
                    num = 1
                if 97 <= ord(letter) <= 122:
                    index = get_letter_index(letter, sorted_letters)
                    if j == 0:
                        row[index] = num
                    else:
                        row[index] = -num
                else:
                    num = int(equation[r][j][l])
                    if j == 0:
                        row[-1] = -num
                    else:
                        row[-1] = num
        matrix.append(row)
    return matrix


def gauss(m):
    nr = len(m)
    ns = len(m[0])

    err = False
    for k in range(nr):

        # find maximum row
        maxVal = abs(m[k][k])
        maxValIdx = k
        for r in range(k, nr):
            if abs(m[r][k]) > maxVal:
                maxVal = abs(m[r][k])
                maxValIdx = r
        # prehodit
        if m[maxValIdx][k] == 0:
            print("cannot proceed .., k=", k)
            err = True
            break
        m[k], m[maxValIdx] = m[maxValIdx], m[k]

        #        print("After swap with row ", maxValIdx)
        #        pm(m)

        for r in range(k + 1, nr):
            f = m[r][k] / m[k][k]
            for s in range(k, ns):
                m[r][s] -= m[k][s] * f
            m[r][k] = 0

    return m


def calc_results(m, sorted_letters):
    res = [0] * len(m)

    for i in range(len(m) - 1, -1, -1):
        summ = 0
        for j in range(i + 1, len(m)):
            summ += res[j] * m[i][j]
        res[i] = (m[i][len(m[0]) - 1] - summ) / m[i][i]
    return res


def print_results(results):
    for result in results:
        if abs(result) < 0.001:
            print(0, end=" ")
        else:
            print(round(result), end=" ")


sorted_letters = get_all_letters(equations)
equations = format_equations(equations)

matrix = make_matrix(equations, sorted_letters)
matrix = gauss(matrix)
print_results(calc_results(matrix, sorted_letters))
