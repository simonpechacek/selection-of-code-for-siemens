import random


def pm(m):
    for r in range(len(m)):
        for s in range(len(m[r])):
            print(m[r][s], end=" ")
        print()


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
    #        print("After row elinmination k=", k)
    #        pm(m)

    # dosadit za x
    x = [0] * nr

    if err:
        return False, x

    for xi in range(nr - 1, -1, -1):
        s = 0
        for j in range(xi + 1, nr):
            s += x[j] * m[xi][j]
        x[xi] = (m[xi][ns - 1] - s) / m[xi][xi]

    return True, x


a = [[0, -1, -1, -5, 3, 4, 0, 40], [-4, 2, -2, -3, -4, 4, 4, 20], [-1, -3, -4, -5, 2, 0, -1, -9],
     [2, 4, 1, 0, 2, 1, 3, 51], [-2, -3, -4, 4, -3, -5, 0, -73], [-2, 3, 2, 1, -1, -5, 3, -19],
     [-2, 3, -5, -3, 0, -3, 2, -1]]

r, x = gauss(a)

for i in x:
    if abs(i) < 1e-3:
        print(0, end=" ")
    else:
        print("{:.0f} ".format(i), end="")
print()
