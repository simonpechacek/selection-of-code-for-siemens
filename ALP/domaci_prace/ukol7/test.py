from decimal import *


def rek_f(x, n):
    if n == 0:
        return Decimal(-1.)
    elif n == 1.:
        return Decimal(x)
    elif n == 2.:
        return Decimal(-(x ** 2))
    if n in cache:
        return Decimal(cache[n])

    cache[n] = Decimal(1 / n) * rek_f(x, n - 1) + Decimal((-1) ** n) * Decimal(n / (n + 1)) * rek_f(x, n - 2) + Decimal((n - 1) / x) * rek_f(x,
                                                                                                                 n - 3)

    return float(cache[n])


n = int(input())
x = float(input())
x_d = Decimal.from_float(x)
n_d = Decimal.from_float(n)
print(x_d)
print(n_d)
cache = {}
print(rek_f(x_d, n))
