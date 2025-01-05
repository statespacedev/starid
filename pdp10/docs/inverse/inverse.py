from copy import deepcopy as dc
import numpy as np

n = 6
ips = np.zeros(n, dtype=np.int32)
m1 = np.zeros((n, n), dtype=np.float32)

def main():
    H, Ha, Hb = dc(m1), dc(m1), dc(m1)
    T, Ta, Tb = dc(m1), dc(m1), dc(m1)
    hil(n, H)
    invhil(n, T)

    invert(n, T, Ha)
    invert(n, T, Hb, imp=True)
    print2(n, Ha)
    print2(n, Hb)
    print2(n, H)

    invert(n, H, Ta)
    invert(n, H, Tb, imp=True)
    print1(n, Ta)
    print1(n, Tb)
    print1(n, T)
    return


def invert(n, A, Ainv, imp=False):
    "for the lu decomposition, calculate the actual inverse matrix"
    LU = dc(m1)
    decomp(n, A, LU)
    for j in range(1, n+1):
        b, x = np.zeros(n, dtype=np.float32), np.zeros(n, dtype=np.float32)
        b[j-1] = 1.
        solve(n, LU, b, x)
        if imp: impruv(n, A, LU, b, x)
        for i in range(1, n+1): Ainv[i-1, j-1] = x[i-1]
    return

def impruv(n, A, LU, b, x):
    "iterative improvement. residuals are small and have to be calculated in double precision."
    eps, itmax = 1e-8, 10
    r, dx = np.zeros(n, dtype=np.float32), np.zeros(n, dtype=np.float32)
    xnorm = np.float32(0.)
    for i in range(1, n+1): xnorm = max(xnorm, abs(x[i-1]))
    if xnorm == 0.:
        digits = -np.log10(eps)
        return digits
    for iter in range(1, itmax+1):
        for i in range(1, n+1):
            sum = np.float64(0.)  # accumulating residual has to be double precision
            for j in range(1, n+1): sum = sum + A[i-1, j-1].astype(np.float64) * x[j-1].astype(np.float64)
            r[i-1] = b[i-1] - sum  # difference of two close numbers
        solve(n, LU, r, dx)
        dxnorm, t = np.float32(0.), np.float32(0.)
        for i in range(1, n + 1):
            t = x[i-1]
            x[i-1] = x[i-1] + dx[i-1]
            dxnorm = max(dxnorm, abs(x[i-1] - t))
        if iter-1 == 0: digits = -np.log10(max(dxnorm / xnorm, eps))
        if iter > itmax:  # dxnorm - eps * xnorm <= 0: 
            return
    sing(3)
    return
            
def decomp(n, A, LU):
    """lower upper lu decomposition of matrix"""
    scales = np.zeros(n, dtype=np.float32)
    for i in range(1, n+1):
        ips[i-1] = i
        rownrm = 0
        for j in range(1, n+1):
            LU[i-1, j-1] = A[i-1, j-1]
            if abs(LU[i-1, j-1]) > rownrm: rownrm = abs(LU[i-1, j-1])
        if rownrm != 0:
            scales[i-1] = 1 / rownrm
        else:
            raise Exception(sing(1))
    nm1 = n - 1
    for k in range(1, nm1+1):
        big, idxpiv = 0, 0
        for i in range(k, n+1):
            ip = ips[i-1]
            size = abs(LU[ip-1, k-1]) * scales[ip-1]
            if size > big:
                big = size
                idxpiv = i
        if big == 0:
            raise Exception(sing(2))
        if idxpiv - k != 0:
            j = ips[k-1]
            ips[k-1] = ips[idxpiv-1]
            ips[idxpiv-1] = j
        kp = ips[k-1]
        pivot = LU[kp-1, k-1]
        kp1 = k + 1
        for i in range(kp1, n+1):
            ip = ips[i-1]
            em = -LU[ip-1, k-1] / pivot
            LU[ip-1, k-1] = -em
            for j in range(kp1, n+1):
                LU[ip-1, j-1] = LU[ip-1, j-1] + em * LU[kp-1, j-1]
    return

def solve(n, LU, b, x):
    """forward and backward substitution to solve for x from LUx = b."""
    ip = ips[1-1]
    x[1-1] = b[ip-1]
    for i in range(2, n+1):
        ip = ips[i-1]
        sum = 0.
        for j in range(1, (i - 1)+1):
            sum = sum + LU[ip-1, j-1] * x[j-1]
        x[i-1] = b[ip-1] - sum
    ip = ips[n-1]
    x[n-1] = x[n-1] / LU[ip-1, n-1]
    for iback in range(2, n+1):
        i = (n + 1) - iback
        ip = ips[i-1]
        sum = 0.
        for j in range((i + 1), n+1):
            sum = sum + LU[ip-1, j-1] * x[j-1]
        x[i-1] = (x[i-1] - sum) / LU[ip-1, i-1]
    return

def sing(iwhy):
    "singular matrix messages"
    if iwhy == 1: return 'matrix with zero row in decompose.'
    elif iwhy == 2: return 'singular matrix in decompose. zero divide in solve.'
    elif iwhy == 3: return 'no convergence in improv. matrix is nearly singular.'

def invhil(n, T):
    """calculate ideal inverse hilbert matrix T"""
    p = n
    for i in range(1, n+1):
        if i != 1: p = ((n - i + 1) * p * (n + i - 1)) / (i - 1)**2
        r = p**2
        T[i-1, i-1] = r / (2 * i - 1)
        for j in range(i+1, n+1):
            r = -((n - j + 1) * r * (n + j - 1)) / (j - 1)**2
            T[j-1, i-1] = r / (i + j - 1)
            T[i-1, j-1] = T[j-1, i-1]

def hil(n, H):
    """calculate ideal hilbert matrix H"""
    for i in range(1, n+1):
        for j in range(1, n+1):
            H[i-1, j-1] = 1 / (i + j - 1)

def print1(n, T):
    """print matrix similar to inverse hilbert T"""
    print()
    for i in range(n):
        for j in range(n):
            print('  %11.2f' % T[i, j], end='')
            if j == n - 1: print()
    return

def print2(n, H):
    """print matrix similar to hilbert H"""
    print()
    for i in range(n):
        for j in range(n):
            print(f'  %11.8f' % H[i, j], end='')
            if j == n - 1: print()
    return

if __name__ == "__main__":
    main()
