      DOUBLE PRECISION SIGMA
      PARAMETER (SIGMA=1.0D70)
      INTEGER MMAX,NMAX,LA,IB,LIWS,LWS
      PARAMETER (MMAX=10,NMAX=30,LA=100,IB=LA*4,LWS=IB+3*MMAX+4,
     +          LIWS=2*IB+10*NMAX+12)
      INTEGER I,IA,ITER,J,JOB,K,M,N,KB,LB,ICNTL(10),INFO(10)
      INTEGER IRN(LA),JCN(LA),IP(NMAX+1),IX(MMAX),JX(NMAX),IWS(LIWS)
      DOUBLE PRECISION A(LA),B(MMAX),C(NMAX),X(NMAX+MMAX),WS(LWS),
     +                 G(NMAX),Z(NMAX),BND(2,NMAX),CNTL(15),RINFO(40),
     +                 CS(NMAX+1)
      EXTERNAL MC59AD,LA04AD

C     Read the data
      READ (5,*) M,N,IA,KB,LB
      READ (5,*) (A(K),IRN(K),JCN(K),K=1,IA)
      READ (5,*) (B(I),I=1,M)
      READ (5,*) (C(J),J=1,N)
      IF (KB.GT.0) READ (5,*) (BND(1,J),BND(2,J),J=1,KB)

C Initialize CNTL
      CALL LA04ID(CNTL)
      CNTL(1) = SIGMA

C Sort the entries by columns
      ICNTL(1) = 0
      ICNTL(2) = 0
      ICNTL(3) = 0
      ICNTL(4) = 6
      ICNTL(5) = 6
      ICNTL(6) = 0
      CALL MC59AD(ICNTL,N,M,IA,IRN,LA,JCN,LA,A,N+1,IP,LIWS,IWS,INFO)

C Scale the problem
      JOB = 0
      CALL LA04AD(A,LA,IRN,IP,M,N,B,C,BND,KB,LB,JOB,CNTL,IX,JX,X,Z,G,
     +            RINFO,WS,LWS,IWS,LIWS)
      DO 10 I = 1,N
        CS(I) = WS(I)*WS(N+M+2)
   10 CONTINUE

C Perform simplex iterations
      JOB = 1
      DO 20 ITER = 1,1000
        CALL LA04AD(A,IA,IRN,IP,M,N,B,C,BND,KB,LB,JOB,CNTL,IX,JX,X,Z,G,
     +              RINFO,WS,LWS,IWS,LIWS)
        IF (JOB.EQ.0) GO TO 40
        IF (JOB.LT.0) GO TO 30
   20 CONTINUE

C Write warning message if unsuccessful
   30 WRITE (6,'(A,I4,A)') ' Solution not found after',ITER,
     +  ' iterations'
      WRITE (6,'(A,F10.6)') ' JOB value is',JOB
      STOP

C Add the bounds in use to X and unscale it
   40 DO 50 I = 1,KB
        IF (JX(I).EQ.1) X(I) = X(I) + BND(1,I)
        IF (JX(I).EQ.2) X(I) = X(I) + BND(2,I)
   50 CONTINUE
      DO 60 I = 1,N
        X(I) = X(I)*CS(I)
   60 CONTINUE

C Write solution
      WRITE (6,'(A,I4,A)') ' Solution found after',ITER,' iterations'
      WRITE (6,'(A,F10.6)') ' Solution value is',RINFO(1)
      WRITE (6,'(A,/(10F10.6))') ' Solution is', (X(J),J=1,N)

      END
