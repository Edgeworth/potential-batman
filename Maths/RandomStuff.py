import numpy as np
import math
import random

/* START SOLUTION */
def gcd(a, b):
  if a == 0: return b
  return gcd(b % a, a)

# a, b = 46, 240
# print(gcd(a, b))

def xgcd(a, b):
  if a == 0: return (0, 1)
  m, n = xgcd(b % a, a)
  return (n - m * (b // a), m)

# m, n = xgcd(a, b)
# print(m, n, m * a + n * b)

def choose_fact(a, b):
  return math.factorial(a) // (math.factorial(a - b) * math.factorial(b))

#print(choose_fact(4, 2))

def fact_mod(a, mod):
  f = 1
  for i in range(2, a + 1):
    f *= i
    f %= mod
  return f

def mod_pow(a, b, mod):
  if b == 0: return 1
  half = mod_pow(a, b // 2, mod)
  full = (half * half) % mod
  if b % 2:
    full *= a
  return full % mod

def mult_inv_fermat(a, mod):
  return mod_pow(a, mod - 2, mod)

def mult_inv_xgcd(a, mod):
  inv, _ = xgcd(a, mod)
  # Careful here since python does negative mods properly, C++ and Java don't.
  return inv % mod

# p = 19
# for i in range(p):
#   print(mult_inv_fermat(i, p), mult_inv_xgcd(i, p))

def choose_fact_mod(a, b, mod):
  num = fact_mod(a, mod)
  den = fact_mod(a - b, mod) * fact_mod(b, mod)
  den %= mod
  # Compute multiplicative inverse and return
  return (num * mult_inv_fermat(den, mod)) % mod

# Careful that mod > n
# pascals(choose_fact, n=15, mod = 19)
# pascals(lambda a, b: choose_fact_mod(a, b, 19), n=15)


def diophantine(a, b, c):
  x, y = xgcd(a, b)
  g = a * x + b * y
  if c % g != 0:
    return None
  x *= c / g
  y *= c / g
  u, v = b // g, a // g
  return [(x + k * u, y - k * v) for k in range(10)]

# a, b, c = 258, 147, 369
# dio = diophantine(a, b, c)
# for x, y in dio:
#   print('%d * %d + %d * %d = %d' % (a, x, b, y, a * x + b * y))


def miller_rabin(p):
  p1 = p - 1
  num_2 = 0
  while p1 % 2 == 0:
    p1 /= 2
    num_2 += 1
  for i in range(20):
    a = random.randint(1, p - 1)
    base = mod_pow(a, p1, p)
    if base == 1: continue
    for k in range(num_2):
      if base == p - 1:
        break
      base *= base
      base %= p
    if base == p - 1:
      continue
    return False
  return True
/* END SOLUTION */
