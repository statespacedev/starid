* COPYRIGHT (c) 1967 AEA Technology
*######DATE 4 Oct 1992
C       Toolpack tool decs employed.
C       SAVE statement for COMMON FA01ED added.
C  EAT 21/6/93 EXTERNAL statement put in for block data on VAXs.
C
C
      DOUBLE PRECISION FUNCTION FA01AD(I)
C     .. Scalar Arguments ..
      INTEGER I
C     ..
C     .. Local Scalars ..
      DOUBLE PRECISION R,S
C     ..
C     .. Intrinsic Functions ..
      INTRINSIC DINT,MOD
C     ..
C     .. Common blocks ..
      COMMON /FA01ED/GL,GR
      DOUBLE PRECISION GL,GR
C     ..
C     .. Data block external statement
      EXTERNAL FA01FD
C     ..
C     .. Save statement ..
      SAVE /FA01ED/
C     ..
C     .. Executable Statements ..
      R = GR*9228907D0/65536D0
      S = DINT(R)
      GL = MOD(S+GL*9228907D0,65536D0)
      GR = R - S
      IF (I.GE.0) FA01AD = (GL+GR)/65536D0
      IF (I.LT.0) FA01AD = (GL+GR)/32768D0 - 1.D0
      GR = GR*65536D0
      RETURN

      END
      SUBROUTINE FA01BD(MAX,NRAND)
C     .. Scalar Arguments ..
      INTEGER MAX,NRAND
C     ..
C     .. External Functions ..
      DOUBLE PRECISION FA01AD
      EXTERNAL FA01AD
C     ..
C     .. Intrinsic Functions ..
      INTRINSIC DBLE,INT
C     ..
C     .. Executable Statements ..
      NRAND = INT(FA01AD(1)*DBLE(MAX)) + 1
      RETURN

      END
      SUBROUTINE FA01CD(IL,IR)
C     .. Scalar Arguments ..
      INTEGER IL,IR
C     ..
C     .. Common blocks ..
      COMMON /FA01ED/GL,GR
      DOUBLE PRECISION GL,GR
C     ..
C     .. Save statement ..
      SAVE /FA01ED/
C     ..
C     .. Executable Statements ..
      IL = GL
      IR = GR
      RETURN

      END
      SUBROUTINE FA01DD(IL,IR)
C     .. Scalar Arguments ..
      INTEGER IL,IR
C     ..
C     .. Common blocks ..
      COMMON /FA01ED/GL,GR
      DOUBLE PRECISION GL,GR
C     ..
C     .. Save statement ..
      SAVE /FA01ED/
C     ..
C     .. Executable Statements ..
      GL = IL
      GR = IR
      RETURN

      END
      BLOCK DATA FA01FD
C     .. Common blocks ..
      COMMON /FA01ED/GL,GR
      DOUBLE PRECISION GL,GR
C     ..
C     .. Save statement ..
      SAVE /FA01ED/
C     ..
C     .. Data statements ..
      DATA GL/21845D0/
      DATA GR/21845D0/
C     ..
C     .. Executable Statements ..
      END
* COPYRIGHT (c) 1988 AEA Technology
*######DATE 21 Jan 1993
C       Toolpack tool decs employed.
C       SAVE statement added.
C 1/10/98 DC(3) not initialized to avoid SUN f90 failure
C 16 October 2001: STOP and WRITE statements removed.

      DOUBLE PRECISION FUNCTION FD05AD(INUM)
C----------------------------------------------------------------
C  Real constants for: IEEE double precision (8-byte arithmetic)
C
C  Obtained from H.S.L. subroutine ZE02AM.
C  Nick Gould and Sid Marlow, Harwell Laboratory, April 1988.
C----------------------------------------------------------------
C     .. Scalar Arguments ..
      INTEGER INUM
C     ..
C     .. Local Arrays ..
      DOUBLE PRECISION DC(5)
C     ..
C     .. Save statement ..
      SAVE DC
C     ..
C     .. Data statements ..
C
C  DC(1) THE SMALLEST POSITIVE NUMBER: 1.0 + DC(1) > 1.0.
C  DC(2) THE SMALLEST POSITIVE NUMBER: 1.0 - DC(2) < 1.0.
C  DC(3) THE SMALLEST NONZERO +VE REAL NUMBER.
C  DC(4) THE SMALLEST FULL PRECISION +VE REAL NUMBER.
C  DC(5) THE LARGEST FINITE +VE REAL NUMBER.
C
      DATA DC(1)/2.2204460492504D-16/
      DATA DC(2)/1.1102230246253D-16/
C     DATA DC(3)/4.9406564584126D-324/
      DATA DC(4)/2.2250738585073D-308/
      DATA DC(5)/1.7976931348622D+308/
C     ..
C     .. Executable Statements ..

      IF ( INUM .LE. 0 ) THEN
         FD05AD = DC( 1 )
      ELSE IF ( INUM .GE. 6 ) THEN
         FD05AD = DC( 5 )
      ELSE IF ( INUM .EQ. 3 ) THEN
         FD05AD = DC(4)/2.0D0**52
      ELSE
         FD05AD = DC( INUM )
      ENDIF
      RETURN
      END
