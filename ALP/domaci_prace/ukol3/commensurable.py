a = int(input())
b = int(input())


def isPrime(num: int):
    # this function determines if number is prime or not
    if num in (0, 1):
        return False

    for i in range(2, int(num ** 0.5) + 1):
        if num % i == 0:
            return False

    return True


def commonDivisor(num_a: int, num_b: int):
    # this function finds the biggest common divisor of num_a and num_b
    if num_a < num_b:
        num_a, num_b = num_b, num_a

    zbytek = num_a % num_b

    while zbytek != 0:
        num_a = num_b
        num_b = zbytek
        zbytek = num_a % num_b
    return num_b


def print_table(num_a, num_b):
    max_num = max(num_a, num_b)
    min_num = min(num_a, num_b)
    size = max_num + 1 - min_num
    for n in range(min_num, max_num+1):
        # sloupce
        for m in range(min_num, max_num+1):
            # radky
            if commonDivisor(m, n) > 1:
                if m == max_num:
                    print("x", end="")
                else:
                    print("x", end="|")
            elif isPrime(m) or isPrime(n):
                if m == max_num:
                    print("p", end="")
                else:
                    print("p", end="|")
            else:
                if m == max_num:
                    print(" ", end="")
                else:
                    print(" ", end="|")
        if n != max_num:
            print("\n" + "-" * (size * 2 - 1))


if a < 2 or b < 2 or a == b:
    print("ERROR")
else:
    print_table(a, b)
