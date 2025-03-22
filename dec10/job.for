c main program
      dimension H(30,30), Ha(30,30), Hb(30,30)
      dimension T(30,30), Ta(30,30), Tb(30,30), ips(30)
      common ips
      common /env/ nout, n
      nout = 5
      n = 6
      call invhil(n, T)
      call hil(n, H)

      call invert(n, T, Ha, 0)
      call invert(n, T, Hb, 1)
      call print2(Ha)
      call print2(Hb)
      call print2(H)

      call invert(n, H, Ta, 0)
      call invert(n, H, Tb, 1)
      call print1(Ta)
      call print1(Tb)
      call print1(T)
      end
      

c print matrix similar to inverse hilbert T
      subroutine print1(T)
      common /env/ nout, n
      dimension T(30,30)
      write (nout,1)
      write (nout,10) ((T(i,j), j = 1,n), i = 1,n)
1     format(1h )
10    format(6f12.2)
      return
      end

c print matrix similar to hilbert H
      subroutine print2(H)
      common /env/ nout, n
      dimension H(30,30)
      write (nout,1)
      write (nout,10) ((H(i,j), j = 1,n), i = 1,n)
1     format(1h )
10    format(6f12.8)
      return
      end

c for the lu decomposition, calculate the actual inverse matrix
      subroutine invert(n, A, Ainv, imp)
      dimension UL(30,30), A(30,30), Ainv(30,30), b(30), x(30)
      call decomp(n, A, UL)
      do 10 j = 1,n
        do 1 i = 1,n
          b(i) = 0.0
1         x(i) = 0.0
        b(j) = 1.0
        call solve(n, UL, b, x)
        if (imp) 4,4,3
3         call impruv(n, A, UL, b, x) 
4       do 5 i = 1,n
5         Ainv(i, j) = x(i)
10    continue      
      return
      end

c iterative improvement. residuals are small and have to be calculated in double precision
      subroutine impruv(n, A, UL, b, x)
      dimension UL(30,30), A(30,30), b(30), x(30), r(30), dx(30)
      double precision sum, t1, t2
      eps = 1.0e-8
      itmax = 10
      xnorm = 0.0
      do 1 i = 1,n
1       xnorm = amax1(xnorm, abs(x(i)))
      if (xnorm) 3,2,3
2       digits = -alog10(eps)
        goto 10
3     do 9 iter = 1,itmax
        do 5 i = 1,n
          sum = 0.0
          do 4 j = 1,n
            t1 = A(i,j)
            t2 = x(j)
4           sum = sum + t1*t2
          r(i) = b(i) - sum
5       continue
        call solve(n, UL, r, dx)
        dxnorm = 0.0
        do 6 i = 1,n
          t = x(i)
          x(i) = x(i) + dx(i)
          dxnorm = amax1(dxnorm, abs(x(i) - t))
6       continue
        if (iter - 1) 8,7,8
7         digits = -alog10(amax1(dxnorm/xnorm,eps))
8       if (iter - itmax) 10,10,9
9     continue
      call sing(3)
10    return
      end

c lower upper lu decomposition of matrix
      subroutine decomp(n, A, UL)
      dimension A(30,30), UL(30,30), scales(30), ips(30)
      common ips
      do 5 i = 1,n
        ips(i) = i
        rownrm = 0.0
        do 2 j = 1,n
          UL(i,j) = A(i,j)
          if (rownrm - abs(UL(i,j))) 1,2,2
1         rownrm = abs(UL(i,j))
2       continue
        if (rownrm) 3,4,3
3       scales(i) = 1.0/rownrm
        go to 5
4       call sing(1)
        scales(i) = 0.0
5     continue
      nm1 = n - 1
      do 17 k = 1,nm1
        big = 0.0
        do 11 i = k,n
          ip = ips(i)
          size = abs(UL(ip,k))*scales(ip)
          if (size - big) 11,11,10
10          big = size
            idxpiv = i
11      continue
        if (big) 13,12,13
12        call sing(2)
          go to 17
13      if (idxpiv - k) 14,15,14
14        j = ips(k)
          ips(k) = ips(idxpiv)
          ips(idxpiv) = j
15      kp = ips(k)
        pivot = UL(kp,k)
        kp1 = k + 1
        do 16 i = kp1,n
          ip = ips(i)
          em = -UL(ip,k)/pivot
          UL(ip,k) = -em
          do 16 j = kp1,n
            UL(ip,j) = UL(ip,j) + em*UL(kp,j)
16      continue
17    continue
      kp = ips(n)
      if (UL(kp,n)) 19,18,19
18    call sing(2)
19    return
      end

c forward and backward substitution to solve for x from ULx = b
      subroutine solve(n, UL, b, x)
      dimension UL(30,30), ips(30), b(30), x(30)
      common ips
      np1 = n + 1
      ip = ips(1)
      x(1) = b(ip)
      do 2 i = 2,n
        ip = ips(i)
        im1 = i - 1
        sum = 0.0
        do 1 j = 1,im1
1         sum = sum + UL(ip,j)*x(j)
2       x(i) = b(ip) - sum
      ip = ips(n)
      x(n) = x(n)/UL(ip,n)
      do 4 iback = 2,n
        i = np1 - iback
        ip = ips(i)
        ip1 = i + 1
        sum = 0.0
        do 3 j = ip1,n
3         sum = sum + UL(ip,j)*x(j)
4     x(i) = (x(i) - sum)/UL(ip,i)
      return
      end

c singular matrix messages
      subroutine sing(iwhy)
      common /env/ nout
11    format(54h0matrix with zero row in decompose                    )
12    format(54h0singular matrix in decompose, zero divide in solve   )
13    format(54h0no convergence in impruv, matrix is nearly singular  )
      go to (1,2,3), iwhy
1     write (nout,11)
      go to 10
2     write (nout,12)
      go to 10
3     write (nout,13)
10    return
      end

c calculate ideal inverse hilbert matrix T
      subroutine invhil(n, T)
      dimension T(30,30)
      p = n
      do 10 i = 1,n
        if (i - 1) 1,2,1
1       p = ((n - i + 1)*p*(n + i - 1))/(i - 1)**2
2       r = p**2
        T(i,i) = r/(2*i - 1)
        do 10 j = i + 1,n
          r = -((n - j + 1)*r*(n + j - 1))/(j - 1)**2
          T(i,j) = r/(i + j - 1)
          T(j,i) = T(i,j)
10    continue
      return
      end

c calculate hilbert matrix Hstar
      subroutine hil(n, Hstar)
      dimension Hstar(30,30)
      do 10 i = 1,n
        do 10 j = 1,n
          Hstar(i,j) = 1.0/(i + j - 1.0)
10    continue
      return
      end

