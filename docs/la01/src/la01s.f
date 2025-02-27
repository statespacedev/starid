* COPYRIGHT (c) 1975 AEA Technology
*######DATE 2 Dec 1992
C       Toolpack tool decs employed.
C       ZERO and ONE made PARAMETERs.
C       LA01F removed from EXTERNAL statement.
C       DELTA reference removed.
C       Dummy argument dimensions changed from (1) to (*).
C       SAVE statements added.
C  EAT 21/6/93 EXTERNAL statement put in for block data so will work on
C       Modified for obsolescent features (Feb 1997)
C
C
C
C    LA01B  - A ROUTINE TO SOLVE THE LINEAR PROGRAMMING PROBLEM.
C      IT CALLS LA01C  TO SOLVE THE PROBLEM, THE MAIN FUNCTION
C      OF LA01B  IS TO SEGMENT THE WORKSPACE PROVIDED BY THE
C      CALLER.
C
      SUBROUTINE LA01B(N,M,L,A,B,C,X,F,IA,IPRINT,IND,WK,IER)
C
C    WK   REAL ARRAY((M+1)*(M+3)); (*:LA01C ); WORKSPACE PROVIDED
C         BY THE CALLER.  LA01B  DIVIDES IT INTO SEGMENTS AND
C         PASSES THEM TO LA01C .
C
C    ALL THE OTHER ARGUMENTS ARE DESCRIBED IN LA01C .
C
C      DEFINE WORKSPACE SEGMENTS
C
C     .. Scalar Arguments ..
      REAL F
      INTEGER IA,IER,IPRINT,L,M,N
C     ..
C     .. Array Arguments ..
      REAL A(IA,*),B(*),C(*),WK(*),X(*)
      INTEGER IND(*)
C     ..
C     .. Local Scalars ..
      INTEGER IU,IV,IW,M1
C     ..
C     .. External Subroutines ..
      EXTERNAL LA01C
C     ..
C     .. Data block external statement
      EXTERNAL LA01F
C     ..
C     .. Executable Statements ..
      M1 = M + 1
      IV = 1
      IW = IV + M1
      IU = IW + M1
C
C      CALL LP ROUTINE
C
      CALL LA01C(N,M,L,A,B,C,X,F,IA,IPRINT,IND,WK(IV),IER,WK(IU),M1,
     +           WK(IW))
C
C      RETURN TO CALLER
C
      RETURN

      END
C@PROCESS DIRECTIVE('IBMD')
C    LA01C  - A ROUTINE TO SOLVE THE LINEAR PROGRAMMING PROBLEM.
C      THE REVISED SIMPLEX METHOD IS USED FURTHER MODIFIED SO
C      THAT IN THE EVENT OF AN INFEASIBLE INITIAL SOLUTION
C      PHASE I IS ENTERED WITH ONLY ONE ARTIFICIAL VARIABLE.
C
      SUBROUTINE LA01C(N,M,L,A,B,C,X,F,IA,IPRINT,IND,V,IER,U,IU,W)
C
C    M      INTEGER SCALAR; (USER:*); NO. OF CONSTRAINTS.
C    N      INTEGER SCALAR; (USER:*); NO. OF USER VARIABLES.
C    L      INTEGER SCALAR; (USER:*); NO. OF INEQUALITIES, L<=M.
C    A      REAL ARRAY(M BY N); (USER:*); THE CONSTRAINT MATRIX,
C           THE L INEQUALITIES MUST BE IN THE FIRST L ROWS.
C    B      REAL ARRAY(M); (USER:*); CONSTRAINT RIGHT HAND SIDES.
C    C      REAL ARRAY(N); (USER:*); OBJECTIVE FUNCTION COEFFS.
C    X      REAL ARRAY(N); (*:LA01C ); THE SOLUTION.
C    F      REAL SCALAR; (*:LA01C ); VALUE OF OBJECTIVE FUNCTION.
C    IA     INTEGER SCALAR; (USER:*); 1ST DIMENSION OF A.
C    IPRINT INTEGER SCALAR; (USER:*); PRINT OPTION,
C             IPRINT=0  SUPPRESS PRINTING
C             IPRINT=1  FINAL RESULT ONLY
C             IPRINT=2  ITERATION FUNCTION VALUES
C             IPRINT=3,4 MORE DETAILED ITERATION PRINT OUTS
C    IND    INTEGER ARRAY(M+1); (USER:LA01C ); BASIS INDICATOR.
C           THE USER MAY FORCE THE INITIAL BASIS BY SETTING NBASIS
C           (IN COMMON LA01D ) AND THE 1ST NBASIS ELEMENTS OF IND.
C           ON RETURN IND WILL HOLD THE FINAL BASIS.
C    V      REAL ARRAY(M+1); (*:LA01C ); WORKSPACE, CONTAINS THE
C           BASIC MARGINAL COSTS ON RETURN.
C    IER    INTEGER SCALAR; (*:LA01C ); ERROR FLAG,
C             IER=0  SUCCESS
C             IER<0  SUCCESS BUT REQUIRED |IER| REINVERSIONS
C             IER=1  ALLOWED NO. OF REINVERSIONS EXEEDED
C             IER=2  MAX BASIC MARGINAL COST > TOLERANCE
C             IER=3  PHASE I FAILED TO FIND A FEASIBLE SOLUTION
C             IER=4  LA01C  FAILED TO FIND M INDEPENDENT COLS
C             IER=5  SOLUTION UNBOUNDED
C             IER=6  INVALID ARGUMENTS (N,M,L)
C    U      REAL ARRAY(M+1 BY M+1); (*:LA01C ); TRANSPOSE OF BASIS
C           INVERSE.
C    IU     INTEGER SCALAR; (USER:*); 1ST DIMENSION OF U
C    W      REAL ARRAY(M+1); (*:LA01C ); WORKSPACE, LP SOLUTION
C
C    COMMON: SEE DESCRIPTION IN BLOCK DATA
C
C
C      CONSTANTS:
C
C      RNDERR: SLIGHT OVER-ESTIMATE OFF ROUNDING ERROR
C      BIG: LARGEST FLOATING POINT NUMBER
C      ONE: THE CONSTANT 1.0
C      ZERO: THE CONSTANT 0.0
C
C***************************************************
C
C      PROLOGUE
C
C*******************************************************
C
C      TEST ARGUMENT VALUES
C     .. Parameters ..
      REAL ONE,ZERO
      PARAMETER (ONE=1.0E0,ZERO=0.0E0)
C     ..
C     .. Scalar Arguments ..
      REAL F
      INTEGER IA,IER,IPRINT,IU,L,M,N
C     ..
C     .. Array Arguments ..
      REAL A(IA,*),B(*),C(*),U(IU,*),V(*),W(*),X(*)
      INTEGER IND(*)
C     ..
C     .. Local Scalars ..
      REAL AMAX,BIG,CINDJ,REDUCE,RNDERR,RVIL,RWID,SCP,SMALL,SUM,THETA,
     +     VI1,VI2,WK1,WK2,Y
      INTEGER I,ICNT,ID,IE,IL,INDJ,INV,IPD,IPHASE,IPR,J,JK,K,M1,ML,NB,
     +        NPR
C     ..
C     .. External Functions ..
      REAL FD05A
      EXTERNAL FD05A
C     ..
C     .. External Subroutines ..
      EXTERNAL LA01E
C     ..
C     .. Intrinsic Functions ..
      INTRINSIC ABS
C     ..
C     .. Common blocks ..
      COMMON /LA01D/EPS,MAXINV,NBASIS,IP,ISP,TOL
      COMMON /LA01G/NAME
      REAL EPS,TOL
      INTEGER IP,ISP,MAXINV,NBASIS
      CHARACTER NAME*8
C     ..
C     .. Save statement ..
      SAVE /LA01D/,/LA01G/
C     ..
C     .. Executable Statements ..
      RNDERR = FD05A(1)
      BIG = FD05A(5)*.1
      IF (TOL.LT.ZERO) TOL = FD05A(1)**.6666667
      IF (N.LE.0) GO TO 540
      IF (M.LE.0) GO TO 540
      IF (L.GT.M) GO TO 540
      IF (M.GT.N+L) GO TO 540
C
C      SET PRINT OPTIONS
      IPR = IPRINT
      IF (IP.LE.0) IPR = 0
      IF (IPR.GE.4) WRITE (IP,FMT=10) NAME,N,M,L

   10 FORMAT ('0',A8,': NO. VARS ',I3,'     NO. CONSTRAINTS ',I4,
     +       '     NO. INEQUALITIES ',I4)
C
C      NO. OF RE-INVERSIONS OF BASIS
      INV = 0
C      ERROR FLAG (0 = NO ERRORS)
      IER = 0
      M1 = M + 1
C
C*****************************************************
C
C      SELECT AND INVERT INITIAL BASIS
C
C******************************************************
C
C      LA01E  BUILDS THE INVERSE A COLUMN AT A TIME PUTTING
C      IN THE SLACKS, IF ANY, FIRST.
C      NOTE: THE INVERSE IS HELD IN U IN TRANSPOSED FORM.
C
C      IF NBASIS > 0 THE USER IS FORCING COLS INTO THE BASIS
C      AND THESE ARE IN IND(J) J=1,NBASIS
C
C
      CALL LA01E(M,N,L,A,IA,NBASIS,IND,U,IU,IE)
C
C      TEST FOR SUCCESS
      IF (IE.GE.0) GO TO 500
C
C***************************************************
C
C      COMPUTE CURRENT SOLUTION, W( ),
C      AND TEST TO SEE IF IT IS FEASIBLE
C
C******************************************************
C
   20 AMAX = ZERO
      DO 30 I = 1,M
        SCP = ZERO
        DO 25 IPD = 1,M
          SCP = SCP + U(IPD,I)*B(IPD)
   25   CONTINUE
        W(I) = SCP
   30 CONTINUE
      DO 35 I = 1,M
C      IF WE HAVE TO INTRODUCE AN ARTIFICIAL VARIABLE WE REMOVE
C      BASIS COL WHICH HAS MAX( ABS(W(I))) TO ENSURE STABILITY
        IF (ABS(W(I)).LE.AMAX) GO TO 35
        AMAX = ABS(W(I))
        ID = I
   35 CONTINUE
C
      IF (AMAX.EQ.ZERO) GO TO 90
      SMALL = -AMAX*RNDERR
      DO 40 I = 1,M
        IF (W(I).LT.SMALL) GO TO 50
   40 CONTINUE
      GO TO 90
C
C***************************************************
C
C      PHASE I
C
C********************************************************
C
C      THE SOLUTION IS NOT FEASIBLE, SO INTRODUCE ONE ARTIFICIAL
C      VARIABLE BY FORCING THE CURRENT SOLUTION TO BE W(I)=1 I=1,M
C
   50 IPHASE = 1
C
C      UPDATE BASIS INDICATOR AND INVERSE
C
      IND(ID) = N + L + 1
      RWID = ONE/W(ID)
      DO 55 J = 1,M
        U(J,ID) = RWID*U(J,ID)
   55 CONTINUE
CIBMD IGNORE RECRDEPS
      DO 65 K = 1,ID - 1
        WK1 = ONE - W(K)
        DO 60 J = 1,M
          U(J,K) = U(J,K) + WK1*U(J,ID)
   60   CONTINUE
   65 CONTINUE
CIBMD IGNORE RECRDEPS
      DO 75 K = ID + 1,M
        WK2 = ONE - W(K)
        DO 70 J = 1,M
          U(J,K) = U(J,K) + WK2*U(J,ID)
   70   CONTINUE
   75 CONTINUE
C
C      SET CURRENT SOLUTION
      DO 80 I = 1,M
        W(I) = ONE
   80 CONTINUE
C
C      SET UP FOR PHASE I
C
C      REDUCE IS USED TO ENSURE THE ARTIFICIAL VARIABLE GETS
C      REMOVED FROM THE BASIS IN THE EVENT OF A TIE.
      REDUCE = ABS(ONE-RNDERR)
C
C      DIMENSION OF INVERSE IS M
      ML = M
      NPR = 4
      GO TO 120
C
C*****************************************************
C
C      PHASE II
C
C*********************************************************
C
C      INTRODUCE OBJECTIVE FUNCTION AND INCREASE DIMENSION
C      OF BASIS TO M + 1.
   90 IPHASE = 2
C      COMPUTE (M+1)TH ROW OF AUGUMENTED BASIS INVERSE
      DO 95 I = 1,M
        U(I,M1) = ZERO
   95 CONTINUE
      DO 110 J = 1,M
        INDJ = IND(J)
        IF (INDJ.GT.N) GO TO 110
        CINDJ = C(INDJ)
        DO 100 I = 1,M
          U(I,M1) = U(I,M1) + CINDJ*U(I,J)
  100   CONTINUE
  110 CONTINUE
C      W(M1) = OBJECTIVE FUNCTION VALUE
      SCP = ZERO
      DO 115 IPD = 1,M
        SCP = SCP + U(IPD,M1)*B(IPD)
  115 CONTINUE
      W(M1) = SCP
C
C      SET UP FOR PHASE II
C
C      DIMENSION OF INVERSE IS M+1
      ML = M1
      NPR = 3
      ID = M1
C
C*********************************************************
C
C      MAIN ITERATION OF THE SIMPLEX METHOD
C
C      THIS CODE IS SHARED BY PHASE I AND PHASE II
C
C*********************************************************
C
C
  120 ICNT = 0
C
      IF (IPR.GE.2) WRITE (IP,FMT=130) NAME,IPHASE

  130 FORMAT ('0',A8,': PHASE ',I1)
C
  140 ICNT = ICNT + 1
C
      IF (IPR.GE.2) WRITE (IP,FMT=150) NAME,ICNT,W(ID)

  150 FORMAT ('0',A8,': ITERATION NO. ',I3,'  COST VALUE ',1P,E15.6)

      IF (IPR.LT.NPR) GO TO 180
      WRITE (IP,FMT=160) NAME

  160 FORMAT (' ',A8,': LP BASIS VARIABLES')

      WRITE (IP,FMT=170) (IND(I),W(I),I=1,M)

  170 FORMAT ((4 ('   ',I3,' ',1P,E23.16)))
C
C      COMPUTE MARGINAL COSTS AND SEARCH FOR A MAXIMUM
C      TO DECIDE WHICH COL IS TO ENTER THE BASIS
C
  180 AMAX = ZERO
      JK = 0
C
C      MARGINAL COSTS FOR COLUMNS OF A
      DO 190 I = 1,N
        SUM = ZERO
        DO 185 IPD = 1,M
          SUM = SUM + U(IPD,ID)*A(IPD,I)
  185   CONTINUE
        IF (IPHASE.EQ.2) SUM = SUM - C(I)
        IF (AMAX.GE.SUM) GO TO 190
        AMAX = SUM
        JK = I
  190 CONTINUE
      IF (L.LE.0) GO TO 210
C
C      MARGINAL COSTS FOR SLACKS
      DO 200 I = 1,L
        IF (AMAX.GE.U(I,ID)) GO TO 200
        AMAX = U(I,ID)
        JK = I + N
  200 CONTINUE
C
C      TEST FOR END OF LP ITERATIONS,
C      ARE ALL MARGINAL COSTS <= 0 ?
  210 IF (JK.LE.0) GO TO 310
C      OR, HAS A BASIC MARGINAL COST BEEN CHOSEN ?
      DO 220 J = 1,M
        IF (JK.EQ.IND(J)) GO TO 310
  220 CONTINUE
C
C      CONTINUE WITH LP ITERATIONS, SAVE CHOSEN MARGINAL COST
      V(M1) = AMAX
C
C      DECIDE WHICH COLUMN IS TO LEAVE THE BASIS
C
      THETA = BIG
      IL = 0
      IF (JK.LE.N) GO TO 240
C
C      INCOMING COLUMN IS A SLACK
      DO 230 I = 1,M
        V(I) = U(JK-N,I)
        IF (V(I).LE.ZERO) GO TO 230
        Y = W(I)/V(I)
        IF (THETA.LE.Y) GO TO 230
        THETA = Y
        IL = I
  230 CONTINUE
      GO TO 260
C
C      INCOMING COLUMN IS FROM A
  240 DO 250 I = 1,M
        SCP = ZERO
        DO 245 IPD = 1,M
          SCP = SCP + U(IPD,I)*A(IPD,JK)
  245   CONTINUE
        V(I) = SCP
  250 CONTINUE
      DO 255 I = 1,M
        IF (V(I).LE.ZERO) GO TO 255
        Y = W(I)/V(I)
        IF (THETA.LE.Y) GO TO 255
        THETA = Y
        IL = I
  255 CONTINUE
C
C
  260 IF (IPHASE.EQ.2) GO TO 270
C      MAKE SURE ARTIFICIAL VARIABLE IS REMOVED IN EVENT OF TIE
      IF (REDUCE*W(ID)/V(ID).GT.THETA) GO TO 270
      IL = ID
  270 IF (IL.EQ.0) GO TO 520
C      UPDATE BASIS INDICATOR
      IND(IL) = JK
C      UPDATE CURRENT LP SOLUTION
      DO 280 I = 1,ML
        W(I) = W(I) - THETA*V(I)
  280 CONTINUE
      W(IL) = THETA
C
C      UPDATE BASIS INVERSE
C
      RVIL = ONE/V(IL)
      DO 285 J = 1,ML
        U(J,IL) = RVIL*U(J,IL)
  285 CONTINUE
CIBMD IGNORE RECRDEPS
      DO 290 I = 1,IL - 1
        VI1 = -V(I)
        DO 300 J = 1,ML
          U(J,I) = U(J,I) + VI1*U(J,IL)
  300   CONTINUE
  290 CONTINUE
CIBMD IGNORE RECRDEPS
      DO 295 I = IL + 1,ML
        VI2 = -V(I)
        DO 305 J = 1,ML
          U(J,I) = U(J,I) + VI2*U(J,IL)
  305   CONTINUE
  295 CONTINUE
C
C      DID WE JUST REMOVE ARTIFICIAL VARIABLE ?
      IF (IL.NE.ID) GO TO 140
C      YES: SWITCH TO PHASE II
      IPHASE = 2
      GO TO 90
C
C*****************************************************
C
C      A NORMAL EXIT FROM THE LP ITERATIONS
C
C*********************************************************
C
C      DO WE RE-INVERT BASIS INVERSE ?
  310 IF (ICNT.LE.1) GO TO 370
      IF (INV.GE.MAXINV) GO TO 350
C
C      YES: SET UP TO RE-INVERT BASIS
      IER = -INV
C      NO. OF RE-INVERSIONS
      INV = INV + 1
      NB = M
      IF (IPHASE.EQ.2) GO TO 320
C      DISCARD ARTIFICIAL VARIABLE BEFORE TRYING AGAIN
      IND(ID) = IND(M)
      NB = M - 1
C
C      RE-INVERT BASIS
  320 CALL LA01E(M,N,L,A,IA,NB,IND,U,IU,IE)
C
C      TEST FOR SUCCESS
      IF (IE.LT.0) GO TO 330
C      NOT SUCCESSFUL, HAVE WE TRIED USING JUST THE COLS THAT
C      ARE INDEPENDENT AND SCANNING THE TABLEAU FOR THE OTHERS ?
      IF (NB.LT.IE) GO TO 500
C      NO: TRY IT THEN
      NB = IE
      GO TO 320
C
  330 IF (IPR.GE.2) WRITE (IP,FMT=340) NAME

  340 FORMAT ('0',A8,': BASIS RE-INVERTED')
C
C      GO AND SEE IF SOLUTION IS STILL FEASIBLE
      GO TO 20
C
C*****************************************************
C
C      WARNING: PERMITTED NO. OF RE-INVERSIONS EXEEDED
C
C******************************************************
C
  350 IF (MAXINV.LE.0) GO TO 370
      IF (ISP.GT.0) WRITE (ISP,FMT=360) NAME,MAXINV

  360 FORMAT ('0',A8,' WARNING: MAX NO. OF ',I3,' BASIS RE-INVERSIONS',
     +       ' EXEEDED')

      IER = 1
C
C*****************************************************
C
C      COMPUTE MARGINAL COSTS ASSOCIATED WITH BASIS
C
C*****************************************************
C
C      COMPUTE MARGINAL COSTS AND FIND MAXIMUM
  370 EPS = ZERO
      DO 400 I = 1,M
C      IGNORE ARTIFICIAL VARIABLE COST (PHASE 1 ONLY)
        IF (I.EQ.ID) GO TO 400
        K = IND(I)
        IF (K.GT.N) GO TO 380
C      COLUMN FROM A
        SCP = ZERO
        DO 375 IPD = 1,M
          SCP = SCP + U(IPD,ID)*A(IPD,K)
  375   CONTINUE
        V(I) = SCP
        IF (IPHASE.EQ.2) V(I) = V(I) - C(K)
        GO TO 390
C      COLUMN FROM SLACKS
  380   V(I) = U(K-N,M1)
  390   IF (ABS(V(I)).LE.EPS) GO TO 400
        EPS = ABS(V(I))
  400 CONTINUE
      V(ID) = W(ID)
C      ARE WE STILL IN PHASE 1 ?
      IF (IPHASE.EQ.1) GO TO 480
C
C      IF LARGEST MARGINAL COST IS > TOL SET ERROR FLAG
      IF (EPS.GT.TOL) IER = 2
C
C      COPY LP SOLUTION INTO CALLER'S ARRAY
C
      DO 410 K = 1,N
        X(K) = ZERO
  410 CONTINUE
      DO 430 J = 1,M
        IF(IND(J)-N.LE.0) X(IND(J)) = W(J)
  430 CONTINUE
C
C      RETURN OBJECTIVE FUNCTION VALUE
C
      F = W(M1)
C
      IF (IPR.EQ.0) GO TO 570
C
C******************************************************
C
C      OUTPUT SECTION FOR FINAL RESULTS
C
C*******************************************************
C
      WRITE (IP,FMT=440) NAME,F

  440 FORMAT ('0',A8,': OPTIMUM COST VALUE ',1P,E25.16)

      WRITE (IP,FMT=450) NAME,IER,EPS

  450 FORMAT ('0',A8,': ERROR FLAGS ',I4,' AND ',1P,E13.6)

      WRITE (IP,FMT=460) NAME

  460 FORMAT ('0',A8,': OPTIMUM SOLUTION')

      WRITE (IP,FMT=470) (I,X(I),I=1,N)

  470 FORMAT ((3 ('   ',I3,' ',1P,E23.16)))

      GO TO 570
C
C******************************************************
C
C      ERROR: PHASE I FAILED TO FIND A FEASIBLE SOLUTION
C
C******************************************************
C
  480 IF (ISP.GT.0) WRITE (ISP,FMT=490) NAME

  490 FORMAT ('0',A8,' ERROR: PHASE I HAS FAILED TO FIND A FEASIBLE',
     +       ' SOLUTION')

      IER = 3
      GO TO 570
C
C*****************************************************
C
C      ERROR: INVERSE BUILDING SECTION FAILED TO FIND
C      M INDEPENDENT COLUMNS FROM THE TABLEAU
C
C********************************************************
C
  500 IF (ISP.GT.0) WRITE (ISP,FMT=510) NAME

  510 FORMAT ('0',A8,' ERROR: RANK OF CONSTRAINT MATRIX < M')

      IER = 4
      GO TO 570
C
C*****************************************************
C
C      ERROR: LP ITERATION PREDICTS AN UNBOUNDED SOLUTION
C
C******************************************************
C
  520 IF (ISP.GT.0) WRITE (ISP,FMT=530) NAME

  530 FORMAT ('0',A8,' WARNING: SOLUTION IS UNBOUNDED')

      IER = 5
      GO TO 570
C
C******************************************************
C
C      ERROR: INVALID ARGUMENTS
C
C******************************************************
C
  540 IF (ISP.LE.0) GO TO 560
      WRITE (ISP,FMT=550) NAME

  550 FORMAT ('0',A8,' ERROR: INVALID ARGUMENT')

      WRITE (ISP,FMT=10) NAME,N,M,L
  560 IER = 6
      GO TO 570
C
C******************************************************
C
C      RETURN TO CALLER (THIS IS THE ONLY EXIT POINT)
C
C*****************************************************
  570 RETURN

      END
C
C*****************************************************
C
C      INITIALIZE THE COMMON BLOCK LA01D
C
C*****************************************************
C
      BLOCK DATA LA01F
C
C     .. Common blocks ..
      COMMON /LA01D/EPS,MAXINV,NBASIS,IP,ISP,TOL
      COMMON /LA01G/NAME
      REAL EPS,TOL
      INTEGER IP,ISP,MAXINV,NBASIS
      CHARACTER NAME*8
C     ..
C     .. Save statement ..
      SAVE /LA01D/,/LA01G/
C     ..
C     .. Data statements ..
      DATA EPS/0.0/,MAXINV/1/,NBASIS/0/,IP/6/,ISP/6/,TOL/-1.0/
      DATA NAME/'  LA01B '/
C     ..
C     .. Executable Statements ..
C
C      THE USER MAY ALTER MAXINV, IP, TOL, NBASIS AND NAME.
C      LA01C  ALWAYS SETS EPS.
C
C    EPS    REAL SCALAR; (*:LA01C ); SET TO THE MAGNITUDE
C           OF THE LARGEST BASIC MARGINAL COST.
C    MAXINV INTEGER SCALAR; (USER:*); LIMIT ON NO. OF REINVERSIONS
C    NBASIS INTEGER SCALAR; (USER:*); NO. OF COLS TO BE FORCED
C           INTO THE INITIAL BASIS.  COLS GIVEN IN ARG IND.
C    IP     INTEGER SCALAR; (USER:*); FORTRAN STREAM NO. FOR OUTPUT
C           SET TO ZERO TO SUPPRESS PRINTING.
C    ISP    INTEGER SCALAR; (USER:*); FORTRAN STREAM NO. FOR
C           DIAGNOSTIC MESSAGES, SET TO ZERO TO SUPPRESS PRINTING.
C    TOL    REAL SCALAR; (USER:*); AN ESTIMATE OF ZERO WHEN
C           TESTING THE ORTHOGONALITY OF THE BASIS AND ITS INVERSE.
C    NAME   IDENTIFIES THE OUTPUT LINES FROM THE ROUTINE.
C
      END
C    LA01E  - GIVEN AN LP TABLEAU CONSISTING OF AN M BY N MATRIX
C      A AUGUMENTED BY THE FIRST L COLUMNS OF THE IDENTITY MATRIX,
C      LA01E  SELECTS M COLUMNS FROM THE TABLEAU THAT FORM A
C      BASIS AND RETURNS THE INVERSE OF THE BASIS IN TRANSPOSED FORM.
C         THE CALLER CAN FORCE COLUMNS INTO THE BASIS BY SETTING
C      NBASIS AND THE ARRAY IND.
C
C@PROCESS DIRECTIVE('IBMD')
      SUBROUTINE LA01E(M,N,L,A,IA,NBASIS,IND,U,IU,IEC)
C
C    M      INTEGER SCALAR; (USER:*); NO. OF ROWS IN MATRIX A.
C    N      INTEGER SCALAR; (USER:*); NO. OF COLS IN MATRIX A.
C    L      INTEGER SCALAR; (USER:*); NO. OF IDENTITY MATRIX COLS.
C    A      REAL ARRAY(M BY N); (USER:*); THE MATRIX A.
C    IA     INTEGER SCALAR; (USER:*); 1ST DIMENSION OF ARRAY A.
C    NBASIS INTEGER SCALAR; (USER:*); THE NO. OF COLS DEFINED
C           IN IND TO BE FORCED INTO THE BASIS.
C    IND    INTEGER ARRAY(M+1); (USER:LA01E ); ON ENTRY GIVES THE
C           COLS TO BE FORCED INTO THE BASIS, ON RETURN IT WILL
C           GIVE THE M COLUMNS THAT WERE PUT INTO THE BASIS.
C           IND(M+1) WILL ALWAYS BE RETURNED SET TO N+L+1.
C    U      REAL ARRAY(M+1 BY M+1); (*:LA01E ); WILL CONTAIN THE TRANSPO
C           OF THE BASIS INVERSE.  LA01E  ALSO SETS THE (M+1)TH ROW TO
C           ZEROS AND U(M+1,M+1) TO ONE, THE REST OF THE (M+1)TH
C           COL IS USED AS WORKSPACE.
C    IU     INTEGER SCALAR; (USER:*); 1ST DIMENSION OF U.
C    IEC    INTEGER SCALAR; (*:LA01E ); ERROR CODE, IT WILL BE
C           SET TO -1 ON A SUCCESSFUL INVERSION AND TO THE
C           NUMBER OF INDEPENDENT COLUMNS IF LA01E  IS UNABLE TO
C           FIND M INDEPENDENT COLS.
C
C
C      CONSTANTS:
C
C     .. Parameters ..
      REAL ONE,ZERO
      PARAMETER (ONE=1.0E0,ZERO=0.0E0)
C     ..
C     .. Scalar Arguments ..
      INTEGER IA,IEC,IU,L,M,N,NBASIS
C     ..
C     .. Array Arguments ..
      REAL A(IA,*),U(IU,*)
      INTEGER IND(*)
C     ..
C     .. Local Scalars ..
      REAL AMAX,RUIM1,SCP,SUM,UKM1,UKM2
      INTEGER I,IPD,ISAVE,J,JK,K,KFLAG,KL,L1,M1,NN
C     ..
C     .. Intrinsic Functions ..
      INTRINSIC ABS
C     ..
C     .. Executable Statements ..
C
      M1 = M + 1
C
C      IS CALLER SUPPLYING ALL OR PART OF THE BASIS ?
      IF (NBASIS.LE.0) GO TO 10
C      YES: SET UP TO SELECT THE FIRST NBASIS COLS FROM IND
      L1 = 1
C      NO. OF COLS TO BE SCANNED
      NN = NBASIS
      KFLAG = -1
      IF (NBASIS.EQ.M) KFLAG = 1
      IND(M1) = N + L + 1
      GO TO 20
C
C      LA01B SELECTS THE BASIS
   10 KFLAG = 0
C      NO. OF COLS TO BE SCANNED
      NN = N
C      ENTER SLACKS PLUS AN EXTRA COLUMN FROM IDENTITY MATRIX
      L1 = L + 1
   20 DO 40 J = 1,L1
        DO 30 I = 1,M
          U(J,I) = ZERO
   30   CONTINUE
        U(J,J) = ONE
        IF (KFLAG.EQ.0) IND(J) = N + J
   40 CONTINUE
C      TEST FOR M SLACKS, IF SO WE HAVE AN INITIAL BASIS
      IF (L1.GT.M) GO TO 170
C
C      IF THE CALLER IS NOT FORCING COLUMNS SELECT M-L  COLUMNS
C      FROM A TO COMPLETE BASIS, OTHERWISE PUT HIS SET IN FIRST
C      AND THEN INCLUDE THE REST OF THE TABLEAU.
C
      DO 160 I = L1,M
C      FOR STABILITY CHOOSE THE COLUMN TO COME IN WHICH
C      HAS  MAX( ABS(ITH ROW OF INVERSE  INTO  COLUMN))
        JK = 0
        AMAX = ZERO
        DO 70 KL = 1,NN
          K = KL
          IF (KFLAG.NE.0) K = IND(KL)
          IF (K.LE.N) GO TO 50
C      SLACK COLUMN
          IF (K-N.GT.I) GO TO 70
          SUM = U(K-N,I)
          GO TO 60
C      COLUMN FROM A
   50     SUM = ZERO
          DO 55 IPD = 1,I
            SUM = SUM + U(IPD,I)*A(IPD,K)
   55     CONTINUE
   60     IF (ABS(SUM).LE.AMAX) GO TO 70
          JK = KL
          AMAX = ABS(SUM)
   70   CONTINUE
C
C      DID WE FIND A COLUMN ?
        IF (JK.EQ.0) GO TO 190
C      YES: ARE WE RE-INVERTING ?
        IF (KFLAG.EQ.0) GO TO 90
C      YES: HAVE WE CHOSEN THIS ONE BEFORE ?
        IF (JK.LT.I) GO TO 190
C      NO: DO WE HAVE TO INTERCHANGE BASIS INDICATORS ?
        IF (JK.EQ.I) GO TO 80
        ISAVE = IND(JK)
        IND(JK) = IND(I)
        IND(I) = ISAVE
   80   JK = IND(I)
        GO TO 110
C      BUILD INVERSE: NOW IF WE HAVE SELECTED A COLUMN THAT WE HAVE
C      ALREADY CHOSEN IT MEANS WE CANNOT FIND M INDEPENDENT COLS.
   90   DO 100 J = 1,I
          IF (JK.EQ.IND(J)) GO TO 190
  100   CONTINUE
C      SET BASIS INDICATORS
        IND(I+1) = IND(I)
        IND(I) = JK
C
C      COMPUTE VECTOR = INVERSE  INTO  SELECTED COLUMN
C      ALSO INITIALIZE (I+1)TH COLUMN OF INVERSE
C
  110   DO 115 J = 1,M
          U(I+1,J) = ZERO
  115   CONTINUE
        IF (JK.LE.N) THEN
          DO 125 J = 1,I
C        COLUMN FROM A
            SCP = ZERO
            DO 120 IPD = 1,I
              SCP = SCP + U(IPD,J)*A(IPD,JK)
  120       CONTINUE
            U(J,M1) = SCP
  125     CONTINUE
          DO 126 J = I + 1,M
C        COLUMN FROM A
            SCP = A(J,JK)
            DO 121 IPD = 1,I
              SCP = SCP + U(IPD,J)*A(IPD,JK)
  121       CONTINUE
            U(J,M1) = SCP
  126     CONTINUE

        ELSE
          DO 130 J = 1,M
C        SLACK COLUMN
            U(J,M1) = U(JK-N,J)
  130     CONTINUE
        END IF

        U(I+1,I+1) = ONE
C
C      UPDATE INVERSE
C
        RUIM1 = ONE/U(I,M1)
        DO 135 J = 1,I
          U(J,I) = U(J,I)*RUIM1
  135   CONTINUE
CIBMD IGNORE RECRDEPS
        DO 145 K = 1,I - 1
          UKM1 = -U(K,M+1)
          DO 140 J = 1,I
            U(J,K) = U(J,K) + UKM1*U(J,I)
  140     CONTINUE
  145   CONTINUE
CIBMD IGNORE RECRDEPS
        DO 155 K = I + 1,M
          UKM2 = -U(K,M+1)
          DO 150 J = 1,I
            U(J,K) = U(J,K) + UKM2*U(J,I)
  150     CONTINUE
  155   CONTINUE
C      IS CALLER FORCING COLS INTO THE BASIS ?
        IF (KFLAG.GE.0) GO TO 160
C      YES: HAVE WE INCLUDED ALL OF THEM YET ?
        IF (I.LT.NBASIS) GO TO 160
C      YES: ARRANGE TO GET THE REST FROM THE LP TABLEAU
        KFLAG = 0
C      THE NO. OF COLS TO BE SCANNED
        NN = N + L
        IND(I+1) = NN + 1
C
  160 CONTINUE
C
C      SET ERROR CODE TO SUCCESS
  170 IEC = -1
C
C      RETURN TO CALLER
C
  180 RETURN
C
C      LA01E  UNABLE TO FIND M INDEPENDENT COLS
C
  190 IEC = I - 1
      GO TO 180

      END
