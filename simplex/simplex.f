      subroutine la01b(n,m,l,a,b,c,x,f,ia,iprint,ind,wk,ier)
      real f
      integer ia,ier,iprint,l,m,n
      real a(ia,*),b(*),c(*),wk(*),x(*)
      integer ind(*)
      integer iu,iv,iw,m1
      external la01c ! is external declaration necessary?
      external la01f
      m1 = m + 1
      iv = 1
      iw = iv + m1
      iu = iw + m1
      call la01c(n,m,l,a,b,c,x,f,ia,iprint,ind,wk(iv),ier,wk(iu),m1,
     + wk(iw))
      return
      end

      subroutine la01c(n,m,l,a,b,c,x,f,ia,iprint,ind,v,ier,u,iu,w)
      real one,zero
      parameter (one=1.0e0,zero=0.0e0)
      real f
      integer ia,ier,iprint,iu,l,m,n
      real a(ia,*),b(*),c(*),u(iu,*),v(*),w(*),x(*)
      integer ind(*)
      real amax,big,cindj,reduce,rnderr,rvil,rwid,scp,small,sum,theta,
     + vi1,vi2,wk1,wk2,y
      integer i,icnt,id,ie,il,indj,inv,ipd,iphase,ipr,j,jk,k,m1,ml,nb,
     + npr
      real fd05a
      external fd05a
      external la01e
      intrinsic abs
      common /la01d/eps,maxinv,nbasis,ip,isp,tol
      common /la01g/name
      real eps,tol
      integer ip,isp,maxinv,nbasis
      character name*8
      save /la01d/,/la01g/
      rnderr = fd05a(1)
      big = fd05a(5)*.1
      if (tol.lt.zero) tol = fd05a(1)**.6666667
      if (n.le.0) go to 540
      if (m.le.0) go to 540
      if (l.gt.m) go to 540
      if (m.gt.n+l) go to 540
      ipr = iprint
      if (ip.le.0) ipr = 0
      if (ipr.ge.4) write (ip,fmt=10) name,n,m,l
   10 format ('0',a8,': no. vars ',i3,'     no. constraints ',i4,
     + '     no. inequalities ',i4)
      inv = 0
      ier = 0
      m1 = m + 1
      call la01e(m,n,l,a,ia,nbasis,ind,u,iu,ie)
      if (ie.ge.0) go to 500
   20 amax = zero
      do 30 i = 1,m
        scp = zero
        do 25 ipd = 1,m
          scp = scp + u(ipd,i)*b(ipd)
   25   continue
        w(i) = scp
   30 continue
      do 35 i = 1,m
        if (abs(w(i)).le.amax) go to 35
        amax = abs(w(i))
        id = i
   35 continue
      if (amax.eq.zero) go to 90
      small = -amax*rnderr
      do 40 i = 1,m
        if (w(i).lt.small) go to 50
   40 continue
      go to 90
   50 iphase = 1
      ind(id) = n + l + 1
      rwid = one/w(id)
      do 55 j = 1,m
        u(j,id) = rwid*u(j,id)
   55 continue
      do 65 k = 1,id - 1
        wk1 = one - w(k)
        do 60 j = 1,m
          u(j,k) = u(j,k) + wk1*u(j,id)
   60   continue
   65 continue
      do 75 k = id + 1,m
        wk2 = one - w(k)
        do 70 j = 1,m
          u(j,k) = u(j,k) + wk2*u(j,id)
   70   continue
   75 continue
      do 80 i = 1,m
        w(i) = one
   80 continue
      reduce = abs(one-rnderr)
      ml = m
      npr = 4
      go to 120
   90 iphase = 2
      do 95 i = 1,m
        u(i,m1) = zero
   95 continue
      do 110 j = 1,m
        indj = ind(j)
        if (indj.gt.n) go to 110
        cindj = c(indj)
        do 100 i = 1,m
          u(i,m1) = u(i,m1) + cindj*u(i,j)
  100   continue
  110 continue
      scp = zero
      do 115 ipd = 1,m
        scp = scp + u(ipd,m1)*b(ipd)
  115 continue
      w(m1) = scp
      ml = m1
      npr = 3
      id = m1
  120 icnt = 0
      if (ipr.ge.2) write (ip,fmt=130) name,iphase
  130 format ('0',a8,': phase ',i1)
  140 icnt = icnt + 1
      if (ipr.ge.2) write (ip,fmt=150) name,icnt,w(id)
  150 format ('0',a8,': iteration no. ',i3,'  cost value ',1p,e15.6)
      if (ipr.lt.npr) go to 180
      write (ip,fmt=160) name
  160 format (' ',a8,': lp basis variables')
      write (ip,fmt=170) (ind(i),w(i),i=1,m)
  170 format ((4 ('   ',i3,' ',1p,e23.16)))
  180 amax = zero
      jk = 0
      do 190 i = 1,n
        sum = zero
        do 185 ipd = 1,m
          sum = sum + u(ipd,id)*a(ipd,i)
  185   continue
        if (iphase.eq.2) sum = sum - c(i)
        if (amax.ge.sum) go to 190
        amax = sum
        jk = i
  190 continue
      if (l.le.0) go to 210
      do 200 i = 1,l
        if (amax.ge.u(i,id)) go to 200
        amax = u(i,id)
        jk = i + n
  200 continue
  210 if (jk.le.0) go to 310
      do 220 j = 1,m
        if (jk.eq.ind(j)) go to 310
  220 continue
      v(m1) = amax
      theta = big
      il = 0
      if (jk.le.n) go to 240
      do 230 i = 1,m
        v(i) = u(jk-n,i)
        if (v(i).le.zero) go to 230
        y = w(i)/v(i)
        if (theta.le.y) go to 230
        theta = y
        il = i
  230 continue
      go to 260
  240 do 250 i = 1,m
        scp = zero
        do 245 ipd = 1,m
          scp = scp + u(ipd,i)*a(ipd,jk)
  245   continue
        v(i) = scp
  250 continue
      do 255 i = 1,m
        if (v(i).le.zero) go to 255
        y = w(i)/v(i)
        if (theta.le.y) go to 255
        theta = y
        il = i
  255 continue
  260 if (iphase.eq.2) go to 270
      if (reduce*w(id)/v(id).gt.theta) go to 270
      il = id
  270 if (il.eq.0) go to 520
      ind(il) = jk
      do 280 i = 1,ml
        w(i) = w(i) - theta*v(i)
  280 continue
      w(il) = theta
      rvil = one/v(il)
      do 285 j = 1,ml
        u(j,il) = rvil*u(j,il)
  285 continue
      do 290 i = 1,il - 1
        vi1 = -v(i)
        do 300 j = 1,ml
          u(j,i) = u(j,i) + vi1*u(j,il)
  300   continue
  290 continue
      do 295 i = il + 1,ml
        vi2 = -v(i)
        do 305 j = 1,ml
          u(j,i) = u(j,i) + vi2*u(j,il)
  305   continue
  295 continue
      if (il.ne.id) go to 140
      iphase = 2
      go to 90
  310 if (icnt.le.1) go to 370
      if (inv.ge.maxinv) go to 350
      ier = -inv
      inv = inv + 1
      nb = m
      if (iphase.eq.2) go to 320
      ind(id) = ind(m)
      nb = m - 1
  320 call la01e(m,n,l,a,ia,nb,ind,u,iu,ie)
      if (ie.lt.0) go to 330
      if (nb.lt.ie) go to 500
      nb = ie
      go to 320
  330 if (ipr.ge.2) write (ip,fmt=340) name
  340 format ('0',a8,': basis re-inverted')
      go to 20
  350 if (maxinv.le.0) go to 370
      if (isp.gt.0) write (isp,fmt=360) name,maxinv
  360 format ('0',a8,' warning: max no. of ',i3,' basis re-inversions',
     + ' exceeded')
      ier = 1
  370 eps = zero
      do 400 i = 1,m
        if (i.eq.id) go to 400
        k = ind(i)
        if (k.gt.n) go to 380
        scp = zero
        do 375 ipd = 1,m
          scp = scp + u(ipd,id)*a(ipd,k)
  375   continue
        v(i) = scp
        if (iphase.eq.2) v(i) = v(i) - c(k)
        go to 390
  380   v(i) = u(k-n,m1)
  390   if (abs(v(i)).le.eps) go to 400
        eps = abs(v(i))
  400 continue
      v(id) = w(id)
      if (iphase.eq.1) go to 480
      if (eps.gt.tol) ier = 2
      do 410 k = 1,n
        x(k) = zero
  410 continue
      do 430 j = 1,m
        if(ind(j)-n.le.0) x(ind(j)) = w(j)
  430 continue
      f = w(m1)
      if (ipr.eq.0) go to 570
      write (ip,fmt=440) name,f
  440 format ('0',a8,': optimum cost value ',1p,e25.16)
      write (ip,fmt=450) name,ier,eps
  450 format ('0',a8,': error flags ',i4,' and ',1p,e13.6)
      write (ip,fmt=460) name
  460 format ('0',a8,': optimum solution')
      write (ip,fmt=470) (i,x(i),i=1,n)
  470 format ((3 ('   ',i3,' ',1p,e23.16)))
      go to 570
  480 if (isp.gt.0) write (isp,fmt=490) name
  490 format ('0',a8,' error: phase i has failed to find a feasible',
     + ' solution')
      ier = 3
      go to 570
  500 if (isp.gt.0) write (isp,fmt=510) name
  510 format ('0',a8,' error: rank of constraint matrix < m')
      ier = 4
      go to 570
  520 if (isp.gt.0) write (isp,fmt=530) name
  530 format ('0',a8,' warning: solution is unbounded')
      ier = 5
      go to 570
  540 if (isp.le.0) go to 560
      write (isp,fmt=550) name
  550 format ('0',a8,' error: invalid argument')
      write (isp,fmt=10) name,n,m,l
  560 ier = 6
      go to 570
  570 return
      end

      block data la01f
      common /la01d/eps,maxinv,nbasis,ip,isp,tol
      common /la01g/name
      real eps,tol
      integer ip,isp,maxinv,nbasis
      character name*8
      save /la01d/,/la01g/
      data eps/0.0/,maxinv/1/,nbasis/0/,ip/6/,isp/6/,tol/-1.0/
      data name/'  la01b '/
      end

      subroutine la01e(m,n,l,a,ia,nbasis,ind,u,iu,iec)
      real one,zero
      parameter (one=1.0e0,zero=0.0e0)
      integer ia,iec,iu,l,m,n,nbasis
      real a(ia,*),u(iu,*)
      integer ind(*)
      real amax,ruim1,scp,sum,ukm1,ukm2
      integer i,ipd,isave,j,jk,k,kflag,kl,l1,m1,nn
      intrinsic abs
      m1 = m + 1
      if (nbasis.le.0) go to 10
      l1 = 1
      nn = nbasis
      kflag = -1
      if (nbasis.eq.m) kflag = 1
      ind(m1) = n + l + 1
      go to 20
   10 kflag = 0
      nn = n
      l1 = l + 1
   20 do 40 j = 1,l1
        do 30 i = 1,m
          u(j,i) = zero
   30   continue
        u(j,j) = one
        if (kflag.eq.0) ind(j) = n + j
   40 continue
      if (l1.gt.m) go to 170
      do 160 i = l1,m
        jk = 0
        amax = zero
        do 70 kl = 1,nn
          k = kl
          if (kflag.ne.0) k = ind(kl)
          if (k.le.n) go to 50
          if (k-n.gt.i) go to 70
          sum = u(k-n,i)
          go to 60
   50     sum = zero
          do 55 ipd = 1,i
            sum = sum + u(ipd,i)*a(ipd,k)
   55     continue
   60     if (abs(sum).le.amax) go to 70
          jk = kl
          amax = abs(sum)
   70   continue
        if (jk.eq.0) go to 190
        if (kflag.eq.0) go to 90
        if (jk.lt.i) go to 190
        if (jk.eq.i) go to 80
        isave = ind(jk)
        ind(jk) = ind(i)
        ind(i) = isave
   80   jk = ind(i)
        go to 110
   90   do 100 j = 1,i
          if (jk.eq.ind(j)) go to 190
  100   continue
        ind(i+1) = ind(i)
        ind(i) = jk
  110   do 115 j = 1,m
          u(i+1,j) = zero
  115   continue
        if (jk.le.n) then
          do 125 j = 1,i
            scp = zero
            do 120 ipd = 1,i
              scp = scp + u(ipd,j)*a(ipd,jk)
  120       continue
            u(j,m1) = scp
  125     continue
          do 126 j = i + 1,m
            scp = a(j,jk)
            do 121 ipd = 1,i
              scp = scp + u(ipd,j)*a(ipd,jk)
  121       continue
            u(j,m1) = scp
  126     continue
        else
          do 130 j = 1,m
            u(j,m1) = u(jk-n,j)
  130     continue
        end if
        u(i+1,i+1) = one
        ruim1 = one/u(i,m1)
        do 135 j = 1,i
          u(j,i) = u(j,i)*ruim1
  135   continue
        do 145 k = 1,i - 1
          ukm1 = -u(k,m+1)
          do 140 j = 1,i
            u(j,k) = u(j,k) + ukm1*u(j,i)
  140     continue
  145   continue
        do 155 k = i + 1,m
          ukm2 = -u(k,m+1)
          do 150 j = 1,i
            u(j,k) = u(j,k) + ukm2*u(j,i)
  150     continue
  155   continue
        if (kflag.ge.0) go to 160
        if (i.lt.nbasis) go to 160
        kflag = 0
        nn = n + l
        ind(i+1) = nn + 1
  160 continue
  170 iec = -1
  180 return
  190 iec = i - 1
      go to 180
      end
	