c main program
      integer vtyps, atbou, bh0
      dimension vtyps(10), atbou(10), bh0(5), tab0(5,11)
      dimension ei1(5,5)
      nout = 6
      call indata(vtyps, atbou, bh0, tab0)
      
      call eimat(ei1, tab0, 6, 5)
      
      write(nout,1) vtyps
1     format(10i5)
      end
      
c eim elementary inverse matrix
      subroutine eimat(ei, tab, incol, pivot)
      dimension ei(5,5), tab(5,11)
      integer incol, pivot
      do 10 j = 1,5
        do 10 i = 1,5
          ei(i,j) = 0
          if (i.eq.j) ei(i,j) = 1
          if (j.ne.pivot) go to 10
          if (i.ne.pivot) ei(i,j) = -tab(i,incol) / tab(pivot,incol)
          if (i.eq.pivot) ei(i,j) = 1 / tab(pivot,incol)
10        continue
      return
      end
      
c assign input data values
      subroutine indata(vtyps, atbou, bh0, tab0)
      integer vtyps, atbou, bh0
      dimension vtyps(10), atbou(10), bh0(5), tab0(5,11)
      do 10 i = 1,10
      atbou(i) = 0
10    vtyps(i) = 2
      vtyps(1) = 3
      vtyps(5) = 1
      vtyps(9) = 1
      vtyps(10) = 1
      do 20 i = 1,5
20    bh0(i) = i
      do 30 j = 1,11
        do 30 i = 1,5
30        tab0(i,j) = 0
      do 40 i = 1,5
40      tab0(i,i) = 1
      tab0(1,6) = -5.4
      tab0(2,6) = .5
      tab0(3,6) = .25
      tab0(4,6) = -1
      tab0(5,6) = .02
      tab0(1,7) = -7.3
      tab0(3,7) = .5
      tab0(4,7) = 1
      tab0(1,8) = -12.96
      tab0(2,8) = .6
      tab0(3,8) = .6
      tab0(1,9) = 120
      tab0(2,9) = -20
      tab0(1,10) = 90
      tab0(3,10) = -10
      tab0(1,11) = -800
      tab0(2,11) = 80
      tab0(3,11) = 40
      tab0(5,11) = 3
      return
      end
      