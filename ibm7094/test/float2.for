$job           job1
$execute       ibjob
$ibjob         
$ibftc deck1   list,m94
c main program
      real x
10    format (f10.5)
      x = -1.23
      write (7, 10) x
      stop
      end
~
$ibsys
$stop
