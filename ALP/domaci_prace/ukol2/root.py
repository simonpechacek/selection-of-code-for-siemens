"""
This program finds root of polynom of 5th degree
Input = a5 - a0 => a5*x**5 ...
x is in interval od (-100, 100)
"""

vals = [float(input()) for _ in range(6)]


def f_x(x):
    y = 0
    for index, coef in enumerate(vals[::-1]):
        y += coef * (x ** index)
    return y


L = -100
R = 100

while abs(R - L) > 1e-9:
    Middle = (L + R) / 2
    vL = f_x(L)
    vR = f_x(R)
    vMid = f_x(Middle)

    if (vL > 0 and vMid > 0) or (vL < 0 and vMid < 0):
        L = Middle

    elif (vR > 0 and vMid > 0) or (vR < 0 and vMid < 0):
        R = Middle
    else:
        break
print(L)
