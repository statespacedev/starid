* COPYRIGHT (c) 1967 AEA Technology
*######DATE 26 Oct 1992
C       New version with S in names.
C  EAT 21/6/93 EXTERNAL statement put in for block data on VAXs.
C  15/10/02 SNGL removed
C
      REAL FUNCTION FA01A(I)
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
      COMMON /FA01E/GL,GR
      DOUBLE PRECISION GL,GR
C     ..
C     .. Save statement ..
      SAVE /FA01E/
C     ..
C     .. Data block external statement
      EXTERNAL FA01F
C     ..
C     .. Executable Statements ..
      R = GR*9228907D0/65536D0
      S = DINT(R)
      GL = MOD(S+GL*9228907D0,65536D0)
      GR = R - S
      IF (I.GE.0) FA01A = (GL+GR)/65536D0
      IF (I.LT.0) FA01A = (GL+GR)/32768D0-1.D0
      GR = GR*65536D0
      RETURN

      END
      SUBROUTINE FA01B(MAX,NRAND)
C     .. Scalar Arguments ..
      INTEGER MAX,NRAND
C     ..
C     .. External Functions ..
      REAL FA01A
      EXTERNAL FA01A
C     ..
C     .. Intrinsic Functions ..
      INTRINSIC FLOAT,INT
C     ..
C     .. Executable Statements ..
      NRAND = INT(FA01A(1)*FLOAT(MAX)) + 1
      RETURN

      END
      SUBROUTINE FA01C(IL,IR)
C     .. Scalar Arguments ..
      INTEGER IL,IR
C     ..
C     .. Common blocks ..
      COMMON /FA01E/GL,GR
      DOUBLE PRECISION GL,GR
C     ..
C     .. Save statement ..
      SAVE /FA01E/
C     ..
C     .. Executable Statements ..
      IL = GL
      IR = GR
      RETURN

      END
      SUBROUTINE FA01D(IL,IR)
C     .. Scalar Arguments ..
      INTEGER IL,IR
C     ..
C     .. Common blocks ..
      COMMON /FA01E/GL,GR
      DOUBLE PRECISION GL,GR
C     ..
C     .. Save statement ..
      SAVE /FA01E/
C     ..
C     .. Executable Statements ..
      GL = IL
      GR = IR
      RETURN

      END
      BLOCK DATA FA01F
C     .. Common blocks ..
      COMMON /FA01E/GL,GR
      DOUBLE PRECISION GL,GR
C     ..
C     .. Save statement ..
      SAVE /FA01E/
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
C 1/10/98 RC(3) not initialized to avoid SUN f90 failure
C 16 October 2001: STOP and WRITE statements removed.

      REAL FUNCTION FD05A( INUM )
      INTEGER INUM
      REAL RC( 5 )
C
C----------------------------------------------------------------
C  Real constants for: IEEE single precision (4-byte arithmetic)
C
C  Obtained from H.S.L. subroutine ZE02AM.
C  Nick Gould and Sid Marlow, Harwell Laboratory, April 1988.
C----------------------------------------------------------------
C
C  OBTAINED FROM H.S.L. SUBROUTINE ZE02AM.
C  NICK GOULD AND SID MARLOW, HARWELL, JULY 1988.
C
C  RC(1) THE 'SMALLEST' POSITIVE NUMBER: 1 + RC(1) > 1.
C  RC(2) THE 'SMALLEST' POSITIVE NUMBER: 1 - RC(2) < 1.
C  RC(3) THE SMALLEST NONZERO +VE REAL NUMBER.
C  RC(4) THE SMALLEST FULL PRECISION +VE REAL NUMBER.
C  RC(5) THE LARGEST FINITE +VE REAL NUMBER.
C
      SAVE RC
      DATA RC(1)/1.19210E-07/
      DATA RC(2)/5.96047E-08/
C     DATA RC(3)/1.40131E-45/
      DATA RC(4)/1.17550E-38/
      DATA RC(5)/3.40281E+38/

      IF ( INUM .LE. 0 ) THEN
         FD05A = RC( 1 )
      ELSE IF ( INUM .GE. 6 ) THEN
         FD05A = RC( 5 )
      ELSE IF ( INUM .EQ. 3 ) THEN
         FD05A = RC(4)/2.0**23
      ELSE
         FD05A = RC( INUM )
      ENDIF
      RETURN
      END
