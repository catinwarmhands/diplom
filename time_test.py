import timeit
import random
import time
from main import *
from haar import *


def current_milli_time():
    return time.time() * 1000


def test(f, count):
    results=[]
    for i in range(count):
        # print(i+1, "/", count)
        t1 = current_milli_time()
        f()
        t2 = current_milli_time()
        results.append(t2-t1)

        # if (i+1)% 100 == 0:
        #     print(t2-t1)
    print("top", sort(results)[-10:], "max", max(results), "min", min(results), "avg", sum(results)/count)

def fasthaar2(a):
    result = [None]*len(a)
    n = int(log2(len(a)))

    for i in range(2**(n-1)):
        summ = a[2*i] + a[2*i+1]
        diff = a[2*i] - a[2*i+1]
        result[i]          = 1/2 * summ
        result[2**(n-1)+i] = 1/2 * diff

    return result

def fasthaar3(a):
    result = [None]*len(a)
    # n = int(log2(len(a)))
    n = len(a)//2

    for i in range(n):
        summ = a[2*i] + a[2*i+1]
        diff = a[2*i] - a[2*i+1]
        result[i]          = 1/2 * summ
        result[n+i] = 1/2 * diff

    return result


count = 1000
size = 5096
a = [random.randint(0,255) for r in range(size)]


test(lambda: fasthaar(a), count)
test(lambda: fasthaar2(a), count)
test(lambda: fasthaar3(a), count)
test(lambda: discreteHaarWaveletTransform(a), count)

CH = hpf_coeffs(CL)
test(lambda: pconv(a, CL, CH, 0), count)