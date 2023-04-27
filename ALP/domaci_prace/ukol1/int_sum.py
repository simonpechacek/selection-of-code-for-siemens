# input = k, a => k = "mocnina", a = "počet čísel" (a > 0 => 0 ... a || a < 0 => a ... 0)
import math

k = int(input())
a = int(input())

def get_sum(exponent, num):
    summ = 0
    if num >= 0:
        for i in range(0, num+1):
            summ += i**exponent
        
        return summ
    else:
        for i in range(num, 1):
            summ += i**exponent
        
        return summ
    
print(get_sum(k, a))