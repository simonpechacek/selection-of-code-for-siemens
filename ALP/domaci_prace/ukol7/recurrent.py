

def rek_f(x, n):
    if n == 0:
        return -1
    elif n == 1.:
        return x
    elif n == 2.:
        return -(x * x)
    if n in cache:
        return cache[n]

    cache[n] = (1 / n) * rek_f(x, n - 1) + ((-1) ** n) * (n / (n + 1)) * rek_f(x, n - 2) + ((n - 1) / x) * rek_f(x,
                                                                                                                 n - 3)

    return cache[n]


n = int(input())
x = float(input())

cache = {}
output = rek_f(x, n)
print("{0:.10f}".format(output))
