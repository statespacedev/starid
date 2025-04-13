$job           job1
$execute       ibjob
$ibjob         
$ibftc deck1   m94
c main program
      j = 1234
      print 2
      write (7,2)
2     format(6h0hello)

      real x, y, x2, y2
      nout = 7
10    format (3hout, 2f10.5)
      y = .00225
      x = sqrt(1. - y**2)
      do 20 i=1,11
      write (7, 10) x, y
c      print 10, x, y
      x2 = x**2 - y**2
      y2 = 2. * x * y
      x = x2
      y = y2
20    continue

      stop
      end
~
$ibsys
$stop
