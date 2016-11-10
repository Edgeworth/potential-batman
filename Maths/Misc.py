import numpy as np
import math
import random
/* START SOLUTION */
def gcd(a, b):
  if a == 0: return b
  return gcd(b % a, a)
a, b = 46, 240
print(gcd(a, b))

def xgcd(a, b):
  if a == 0: return (0, 1)
  m, n = xgcd(b % a, a)
  return (n - m * (b // a), m)
m, n = xgcd(a, b)
print(m, n, m * a + n * b)

def sieve(n):
  s = [0, 0] + [1] * n
  primes = []
  for p, is_p in enumerate(s):
    if not is_p:
      continue
    i = p * p
    while i < len(s):
      s[i] = 0
      i += p
    primes.append(p)
  return primes
print(sieve(100))

# Fermats: a^(p - 1) = 1 mod p
# Euler's: a^totient(n) = 1 mod n - any n; totient(n) = # coprime in (1, n)
def mult_inv_fermat(a, mod):
  return mod_pow(a, mod - 2, mod)
def mult_inv_xgcd(a, mod):
  inv, _ = xgcd(a, mod)
  # Careful here since python does negative mods properly, C++ and Java don't.
  return inv % mod
# Careful that mod > n
def choose_fact_mod(a, b, mod):
  num = fact_mod(a, mod)
  den = fact_mod(a - b, mod) * fact_mod(b, mod)
  den %= mod
  # Compute multiplicative inverse and return
  return (num * mult_inv_fermat(den, mod)) % mod


def diophantine(a, b, c):
  x, y = xgcd(a, b)
  g = a * x + b * y
  if c % g != 0:
    return None
  x *= c / g
  y *= c / g
  u, v = b // g, a // g
  return [(x + k * u, y - k * v) for k in range(10)]
a, b, c = 258, 147, 369
dio = diophantine(a, b, c)
for x, y in dio:
  print('%d * %d + %d * %d = %d' % (a, x, b, y, a * x + b * y))

# Probabilistic
def miller_rabin(p):
  p1 = p - 1
  num_2 = 0
  while p1 % 2 == 0:
    p1 /= 2
    num_2 += 1
  for i in range(20):  # Increase this for extra sure-ness
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
print(' '.join(str(i) for i in range(2, 100) if miller_rabin(i)))
/* END SOLUTION */
