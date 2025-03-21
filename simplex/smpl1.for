
c main program
      integer vtyps, atbou, bh
      dimension vtyps(10), atbou(10), bh(5), tab(5,11) 
      nout = 6
      call init(vtyps, atbou, bh, tab)
      call updt(tab, bh, 6, 5)
      call updt(tab, bh, 8, 3)
      call updt(tab, bh, 10, 2)
      atbou(10) = 1
      tab(2,11) = tab(2,11) - 1.0
      call updt(tab, bh, 5, 2)
      write(nout,10) ((tab(i,j), j = 1,11), i = 1,5)
10    format(11f8.2)
      end
      
c update tableau 
      subroutine updt(tabb, bh, col, piv)
      integer col, piv, bh
      dimension bh(5), eim(5,5), tabb(5,11), taba(5,11)
      bh(piv) = col
      taba = tabb
      do 10 j = 1,5
        do 10 i = 1,5
          eim(i,j) = 0
          if (i.eq.j) eim(i,j) = 1
          if (j.ne.piv) go to 10
          if (i.ne.piv) eim(i,j) = -taba(i,col) / taba(piv,col)
          if (i.eq.piv) eim(i,j) = 1 / taba(piv,col)
10        continue
      do 20 j = 1,11
        do 20 i = 1,5
          tabb(i,j) = 0
          do 15 k = 1,5
15          tabb(i,j) = tabb(i,j) + eim(i,k)*taba(k,j)
20    continue
      return
      end
      
c initial
      subroutine init(vtyps, atbou, bh, tab)
      integer vtyps, atbou, bh
      dimension vtyps(10), atbou(10), bh(5), tab(5,11)
      do 10 i = 1,10
      atbou(i) = 0
10    vtyps(i) = 2
      vtyps(1) = 3
      vtyps(5) = 1
      vtyps(9) = 1
      vtyps(10) = 1
      do 20 i = 1,5
20    bh(i) = i
      do 30 j = 1,11
        do 30 i = 1,5
30        tab(i,j) = 0
      do 40 i = 1,5
40      tab(i,i) = 1
      tab(1,6) = -5.4
      tab(2,6) = .5
      tab(3,6) = .25
      tab(4,6) = -1
      tab(5,6) = .02
      tab(1,7) = -7.3
      tab(3,7) = .5
      tab(4,7) = 1
      tab(1,8) = -12.96
      tab(2,8) = .6
      tab(3,8) = .6
      tab(1,9) = 120
      tab(2,9) = -20
      tab(1,10) = 90
      tab(3,10) = -10
      tab(1,11) = -800
      tab(2,11) = 80
      tab(3,11) = 40
      tab(5,11) = 3
      return
      end
      