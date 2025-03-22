$job           job1
$execute       ibjob
$ibjob         
$ibftc deck1   m94
c main program
      dimension H(6,6)
      call hil(H)
      call print2(H)
      stop
      end
$ibftc deck10   m94
c calculate hilbert matrix Hstar
      subroutine hil(Hstar)
      dimension Hstar(6,6)
      do 10 i = 1,6
        do 10 j = 1,6
          a = i
          b = j
          Hstar(i,j) = 1.0/(a + b - 1.0)
10    continue
      return
      end
$ibftc deck3   m94
c print matrix similar to hilbert H
      subroutine print2(H)
      dimension H(6,6)
      write (7,10) ((H(i,j), j = 1,6), i = 1,6)
c      print 10, ((H(i,j), j = 1,6), i = 1,6)
10    format(3hout,6f12.8)
      return
      end
~
$ibsys
$stop
