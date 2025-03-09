$job           test1
$execute       ibjob
$ibjob         map                                               
$ibftc deck1   list,dd,nodeck,m94
c main program
      integer i
      real x, y, x2, y2
10    format (2f10.5)
      y = .00225
      x = sqrt(1. - y**2)
      do 20 i=1,11
      print 10, x, y
      write (7,10) x, y
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
