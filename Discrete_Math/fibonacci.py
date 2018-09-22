# Discrete Math
# Problem 1: Fibonacci sequence 

def fib(number):
    if (number <= 1):
        return number
    
    return fib(number - 1) + fib(number - 2)

print(fib(8))
# 1 1 2 3 5 8 13 21 