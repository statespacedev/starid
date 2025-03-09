$job           test2
$execute       fortran
$block         bcd,0084
*     xeq
c main program
c      integer i
c      real x, y, x2, y2
10    format (2f10.5)
      y = .00225
      x = sqrt(1. - y**2)
      do 20 i=1,11
      print 10, x, y
c      write (7,10) x, y
      x2 = x**2 - y**2
      y2 = 2. * x * y
      x = x2
      y = y2
20    continue
      call exit
      end
~
$ibsys
$stop