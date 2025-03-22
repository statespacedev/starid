c$job           job1
c$execute       ibjob
c$ibjob         
c$ibftc deck1   m94
c imaginary powers of 10, starting with 10**(i/1024),
c and squaring successively ten times. feynman's table 22-3. 
      real x, y, x2, y2
      nout = 6
10    format (3hout, 2f10.5)
      y = .00225
      x = sqrt(1. - y**2)
      do 20 i=1,11
        write (nout, 10) x, y
        x2 = x**2 - y**2
        y2 = 2. * x * y
        x = x2
        y = y2
20    continue
      stop
      end
c~
c$ibsys
c$stop
