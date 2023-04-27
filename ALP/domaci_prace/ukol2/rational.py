"""
This program converts any Real number from Decimal to any other system with base (2-36)
Input: base (2-36), Real number
Output: Real number with new base with 10 decimal places
"""

base = int(input())
is_negative = False
realNumber = float(input())
if realNumber < 0:
    is_negative = True
    realNumber = abs(realNumber)
base_letters = {}


def create_base(base):
    for i in range(base):
        if i < 10:
            base_letters.update({i: i})
        else:
            base_letters.update({i: chr(ord("a") + i - 10)})


def getN(num, base):
    n = 0
    while base ** (n + 1) <= num:
        n += 1
    return n


create_base(base)
n = getN(realNumber, base)


def convert(num, base, n):
    number = ""
    if is_negative:
        number += "-"
    while n > -7:
        lett_num = num // base ** n
        num -= (base ** n) * lett_num
        number += str(base_letters[lett_num])
        if n == 0:
            number += "."

        n -= 1
    return number


print(convert(realNumber, base, n))
