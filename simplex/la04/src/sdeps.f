* COPYRIGHT (c) 1988 AEA Technology
* Original date 17 Feb 2005

C 17th February 2005 Version 1.0.0. Replacement for FD05.

      REAL FUNCTION FD15A(T)
C----------------------------------------------------------------
C  Fortran 77 implementation of the Fortran 90 intrinsic
C    functions: EPSILON, TINY, HUGE and RADIX.  Note that
C    the RADIX result is returned as REAL.
C
C  The CHARACTER argument specifies the type of result:
C       
C   'E'  smallest positive real number: 1.0 + FD15A > 1.0, i.e.
C          EPSILON(REAL)
C   'T'  smallest full precision positive real number, i.e.
C          TINY(REAL)
C   'H'  largest finite positive real number, i.e.
C          HUGE(REAL)
C   'R'  the base of the floating point arithematic, i.e.
C          RADIX(REAL)
C
C    any other value gives a result of zero.
C----------------------------------------------------------------
      CHARACTER T

      IF ( T.EQ.'E' ) THEN
         FD15A = EPSILON(1.0)
      ELSE IF ( T.EQ.'T' ) THEN
         FD15A = TINY(1.0)
      ELSE IF ( T.EQ.'H' ) THEN
         FD15A = HUGE(1.0)
      ELSE IF ( T.EQ.'R' ) THEN
         FD15A = REAL(RADIX(1.0))
      ELSE
         FD15A = 0.0
      ENDIF
      RETURN
      END
* COPYRIGHT (c) 1993 Council for the Central Laboratory
*                    of the Research Councils

C Original date 29 Jan 2001
C 29 January 2001. Modified from MC49 to be threadsafe.

C 12th July 2004 Version 1.0.0. Version numbering added.
C 28 February 2008. Version 1.0.1. Comments flowed to column 72.
C 21 September 2009. Version 1.0.2. Minor change to documentation.

      SUBROUTINE MC59A(ICNTL,NC,NR,NE,IRN,LJCN,JCN,LA,A,LIP,IP,
     &                  LIW,IW,INFO)
C
C To sort the sparsity pattern of a matrix to an ordering by columns.
C There is an option for ordering the entries within each column by
C increasing row indices and an option for checking the user-supplied
C matrix entries for indices which are out-of-range or duplicated.
C
C ICNTL:  INTEGER array of length 10. Intent(IN). Used to specify
C         control parameters for the subroutine.
C ICNTL(1): indicates whether the user-supplied matrix entries are to
C           be checked for duplicates, and out-of-range indices.
C           Note  simple checks are always performed.
C           ICNTL(1) = 0, data checking performed.
C           Otherwise, no data checking.
C ICNTL(2): indicates the ordering requested.
C           ICNTL(2) = 0, input is by rows and columns in arbitrary
C           order and the output is sorted by columns.
C           ICNTL(2) = 1, the output is also row ordered
C           within each column.
C           ICNTL(2) = 2, the input is already ordered by
C           columns and is to be row ordered within each column.
C           Values outside the range 0 to 2 are flagged as an error.
C ICNTL(3): indicates whether matrix entries are also being ordered.
C           ICNTL(3) = 0, matrix entries are ordered.
C           Otherwise, only the sparsity pattern is ordered
C           and the array A is not accessed by the routine.
C ICNTL(4): the unit number of the device to
C           which error messages are sent. Error messages
C           can be suppressed by setting ICNTL(4) < 0.
C ICNTL(5): the unit number of the device to
C           which warning messages are sent. Warning
C           messages can be suppressed by setting ICNTL(5) < 0.
C ICNTL(6)  indicates whether matrix symmetric. If unsymmetric, ICNTL(6)
C           must be set to 0.
C           If ICNTL(6) = -1 or 1, symmetric and only the lower
C           triangular part of the reordered matrix is returned.
C           If ICNTL(6) = -2 or 2, Hermitian and only the lower
C           triangular part of the reordered matrix is returned.
C           If error checks are performed (ICNTL(1) = 0)
C           and ICNTL(6)> 1 or 2, the values of duplicate
C           entries are added together; if ICNTL(6) < -1 or -2, the
C           value of the first occurrence of the entry is used.
C ICNTL(7) to ICNTL(10) are not currently accessed by the routine.
C
C NC:      INTEGER variable. Intent(IN). Must be set by the user
C          to the number of columns in the matrix.
C NR:      INTEGER variable. Intent(IN). Must be set by the user
C          to the number of rows in the matrix.
C NE:      INTEGER variable. Intent(IN). Must be set by the user
C          to the number of entries in the matrix.
C IRN: INTEGER array of length NE. Intent (INOUT). Must be set by the
C            user to hold the row indices of the entries in the matrix.
C          If ICNTL(2).NE.2, the entries may be in any order.
C          If ICNTL(2).EQ.2, the entries in column J must be in
C            positions IP(J) to IP(J+1)-1 of IRN. On exit, the row
C            indices are reordered so that the entries of a single
C            column are contiguous with column J preceding column J+1, J
C            = 1, 2, ..., NC-1, with no space between columns.
C          If ICNTL(2).EQ.0, the order within each column is arbitrary;
C            if ICNTL(2) = 1 or 2, the order within each column is by
C            increasing row indices.
C LJCN:    INTEGER variable. Intent(IN). Defines length array
C JCN:     INTEGER array of length LJCN. Intent (INOUT).
C          If ICNTL(2) = 0 or 1, JCN(K) must be set by the user
C          to the column index of the entry
C          whose row index is held in IRN(K), K = 1, 2, ..., NE.
C          On exit, the contents of this array  will have been altered.
C          If ICNTL(2) = 2, the array is not accessed.
C LA:      INTEGER variable. Intent(IN). Defines length of array
C          A.
C A:       is a REAL (DOUBLE PRECISION in the D version, INTEGER in
C          the I version, COMPLEX in the C version,
C          or COMPLEX"*"16 in the Z version) array of length LA.
C          Intent(INOUT).
C          If ICNTL(3).EQ.0, A(K) must be set by the user to
C          hold the value of the entry with row index IRN(K),
C          K = 1, 2, ..., NE. On exit, the array will have been
C          permuted in the same way as the array IRN.
C          If ICNTL(3).NE.0, the array is not accessed.
C LIP:     INTEGER variable. Intent(IN). Defines length of array
C          IP.
C IP:      INTEGER array of length LIP. Intent(INOUT). IP
C          need only be set by the user if ICNTL(2) = 2.
C          In this case, IP(J) holds the position in
C          the array IRN of the first entry in column J, J = 1, 2,
C          ..., NC, and IP(NC+1) is one greater than the number of
C          entries in the matrix.
C          In all cases, the array IP will have this meaning on exit
C          from the subroutine and is altered when ICNTL(2) = 2 only
C          when ICNTL(1) =  0 and there are out-of-range
C          indices or duplicates.
C LIW:     INTEGER variable. Intent(IN). Defines length of array
C          IW.
C IW:      INTEGER array of length LIW. Intent(OUT). Used by the
C          routine as workspace.
C INFO:    INTEGER array of length 10.  Intent(OUT). On exit,
C          a negative value of INFO(1) is used to signal a fatal
C          error in the input data, a positive value of INFO(1)
C          indicates that a warning has been issued, and a
C          zero value is used to indicate a successful call.
C          In cases of error, further information is held in INFO(2).
C          For warnings, further information is
C          provided in INFO(3) to INFO(6).  INFO(7) to INFO(10) are not
C          currently used and are set to zero.
C          Possible nonzero values of INFO(1):
C         -1 -  The restriction ICNTL(2) = 0, 1, or 2 violated.
C               Value of ICNTL(2) is given by INFO(2).
C         -2 -  NC.LE.0. Value of NC is given by INFO(2).
C         -3 -  Error in NR. Value of NR is given by INFO(2).
C         -4 -  NE.LE.0. Value of NE is given by INFO(2).
C         -5 -  LJCN too small. Min. value of LJCN is given by INFO(2).
C         -6 -  LA too small. Min. value of LA is given by INFO(2).
C         -7 -  LIW too small. Value of LIW is given by INFO(2).
C         -8 -  LIP too small. Value of LIP is given by INFO(2).
C         -9 -  The entries of IP not monotonic increasing.
C        -10 -  For each I, IRN(I) or JCN(I) out-of-range.
C        -11 -  ICNTL(6) is out of range.
C         +1 -  One or more duplicated entries. One copy of
C               each such entry is kept and, if ICNTL(3) = 0 and
C               ICNTL(6).GE.0, the values of these entries are
C               added together. If  ICNTL(3) = 0 and ICNTL(6).LT.0,
C               the value of the first occurrence of the entry is used.
C               Initially INFO(3) is set to zero. If an entry appears
C               k times, INFO(3) is incremented by k-1 and INFO(6)
C               is set to the revised number of entries in the
C               matrix.
C         +2 - One or more of the entries in IRN out-of-range. These
C               entries are removed by the routine.`INFO(4) is set to
C               the number of entries which were out-of-range and
C               INFO(6) is set to the revised number of entries in the
C               matrix.
C         +4 - One or more of the entries in JCN out-of-range. These
C               entries are removed by the routine. INFO(5) is set to
C               the number of entries which were out-of-range and
C               INFO(6) is set to the revised number of entries in the
C               matrix. Positive values of INFO(1) are summed so that
C               the user can identify all warnings.
C
C     .. Scalar Arguments ..
      INTEGER LA,LIP,LIW,LJCN,NC,NE,NR
C     ..
C     .. Array Arguments ..
      REAL A(LA)
      INTEGER ICNTL(10),IP(LIP),INFO(10),IRN(NE),IW(LIW),JCN(LJCN)
C     ..
C     .. Local Scalars ..
      INTEGER I,ICNTL1,ICNTL2,ICNTL3,ICNTL6,LAA
      INTEGER IDUP,IOUT,IUP,JOUT,LP,MP,KNE,PART
      LOGICAL LCHECK
C     ..
C     .. External Subroutines ..
      EXTERNAL MC59B,MC59C,MC59D,MC59E,MC59F
C     ..
C     .. Intrinsic Functions ..
      INTRINSIC MAX
C     ..
C     .. Executable Statements ..

C Initialise
      DO 10 I = 1,10
         INFO(I) = 0
   10 CONTINUE

      ICNTL1 = ICNTL(1)
      ICNTL2 = ICNTL(2)
      ICNTL3 = ICNTL(3)
      ICNTL6 = ICNTL(6)
      LCHECK = (ICNTL1.EQ.0)
C Streams for errors/warnings
      LP = ICNTL(4)
      MP = ICNTL(5)

C  Check the input data
      IF (ICNTL2.GT.2 .OR. ICNTL2.LT.0) THEN
         INFO(1) = -1
         INFO(2) = ICNTL2
         IF (LP.GT.0) THEN
            WRITE (LP,FMT=9000) INFO(1)
            WRITE (LP,FMT=9010) ICNTL2
         END IF
         GO TO 70
      END IF

      IF (ICNTL6.GT.2 .OR. ICNTL6.LT.-2) THEN
         INFO(1) = -11
         INFO(2) = ICNTL6
         IF (LP.GT.0) THEN
            WRITE (LP,FMT=9000) INFO(1)
            WRITE (LP,FMT=9150) ICNTL6
         END IF
         GO TO 70
      END IF
C For real matrices, symmetric = Hermitian so only
C have to distinguish between unsymmetric (ICNTL6 = 0) and
C symmetric (ICNTL6.ne.0)

      IF (NC.LT.1) THEN
        INFO(1) = -2
        INFO(2) = NC
        IF (LP.GT.0) THEN
          WRITE (LP,FMT=9000) INFO(1)
          WRITE (LP,FMT=9020) NC
        END IF
        GO TO 70
      END IF

      IF (NR.LT.1) THEN
        INFO(1) = -3
        INFO(2) = NR
        IF (LP.GT.0) THEN
          WRITE (LP,FMT=9000) INFO(1)
          WRITE (LP,FMT=9030) NR
        END IF
        GO TO 70
      END IF

      IF (ICNTL6.NE.0 .AND. NR.NE.NC) THEN
        INFO(1) = -3
        INFO(2) = NR
        IF (LP.GT.0) THEN
          WRITE (LP,FMT=9000) INFO(1)
          WRITE (LP,FMT=9035) NC,NR
        END IF
        GO TO 70
      END IF

      IF (NE.LT.1) THEN
        INFO(1) = -4
        INFO(2) = NE
        IF (LP.GT.0) THEN
          WRITE (LP,FMT=9000) INFO(1)
          WRITE (LP,FMT=9040) NE
        END IF
        GO TO 70
      END IF

      IF (ICNTL2.EQ.0 .OR. ICNTL2.EQ.1) THEN
        IF (LJCN.LT.NE) THEN
          INFO(1) = -5
          INFO(2) = NE
        END IF
      ELSE
        IF (LJCN.LT.1) THEN
          INFO(1) = -5
          INFO(2) = 1
        END IF
      END IF
      IF (INFO(1).EQ.-5) THEN
         IF (LP.GT.0) THEN
            WRITE (LP,FMT=9000) INFO(1)
            WRITE (LP,FMT=9050) LJCN,INFO(2)
         END IF
         GO TO 70
      END IF

      IF (ICNTL3.EQ.0) THEN
        IF (LA.LT.NE) THEN
          INFO(1) = -6
          INFO(2) = NE
        END IF
      ELSE
        IF (LA.LT.1) THEN
          INFO(1) = -6
          INFO(2) = 1
        END IF
      END IF
      IF (INFO(1).EQ.-6) THEN
         IF (LP.GT.0) THEN
            WRITE (LP,FMT=9000) INFO(1)
            WRITE (LP,FMT=9060) LA,INFO(2)
         END IF
         GO TO 70
      END IF

      IF (ICNTL2.EQ.0 .OR. ICNTL2.EQ.2) THEN
        IF (LIP.LT.NC+1) THEN
          INFO(1) = -7
          INFO(2) = NC+1
        END IF
      ELSE IF (LIP.LT.MAX(NR,NC)+1) THEN
        INFO(1) = -7
        INFO(2) = MAX(NR,NC)+1
      END IF
      IF (INFO(1).EQ.-7) THEN
        IF (LP.GT.0) THEN
          WRITE (LP,FMT=9000) INFO(1)
          WRITE (LP,FMT=9065) LIP,INFO(2)
        END IF
        GO TO 70
      END IF

C Check workspace sufficient
      IF (LIW.LT.MAX(NR,NC)+1) THEN
        INFO(1) = -8
        INFO(2) = MAX(NR,NC)+1
        IF (LP.GT.0) THEN
          WRITE (LP,FMT=9000) INFO(1)
          WRITE (LP,FMT=9070) LIW,INFO(2)
        END IF
        GO TO 70
      END IF

      LAA = NE
      IF (ICNTL3.NE.0) LAA = 1
C Initialise counts of number of out-of-range entries and duplicates
      IOUT = 0
      JOUT = 0
      IDUP = 0
      IUP = 0

C PART is used by MC59B to indicate if upper or lower or
C all of matrix is required.
C PART =  0 : unsymmetric case, whole matrix wanted
C PART =  1 : symmetric case, lower triangular part of matrix wanted
C PART = -1 : symmetric case, upper triangular part of matrix wanted
      PART = 0
      IF (ICNTL6.NE.0) PART = 1

      IF (ICNTL2.EQ.0) THEN

C Order directly by columns
C On exit from MC59B, KNE holds number of entries in matrix
C after removal of out-of-range entries. If no data checking, KNE = NE.
        CALL MC59B(LCHECK,PART,NC,NR,NE,IRN,JCN,LAA,A,IP,IW,
     +              IOUT,JOUT,KNE)
C Return if ALL entries out-of-range.
        IF (KNE.EQ.0) GO TO 50

C Check for duplicates
        IF (LCHECK) CALL MC59E(NC,NR,NE,IRN,LIP,IP,LAA,A,IW,IDUP,
     &                          KNE,ICNTL6)

      ELSE IF (ICNTL2.EQ.1) THEN

C First order by rows.
C Interchanged roles of IRN and JCN, so set PART = -1
C if matrix is symmetric case
        IF (ICNTL6.NE.0) PART = -1
        CALL MC59B(LCHECK,PART,NR,NC,NE,JCN,IRN,LAA,A,IW,IP,
     +              JOUT,IOUT,KNE)
C Return if ALL entries out-of-range.
        IF (KNE.EQ.0) GO TO 50

C At this point, JCN and IW hold column indices and row pointers
C Optionally, check for duplicates.
        IF (LCHECK) CALL MC59E(NR,NC,NE,JCN,NR+1,IW,LAA,A,IP,
     &                          IDUP,KNE,ICNTL6)

C Now order by columns and by rows within each column
        CALL MC59C(NC,NR,KNE,IRN,JCN,LAA,A,IP,IW)

      ELSE IF (ICNTL2.EQ.2) THEN
C Input is using IP, IRN.
C Optionally check for duplicates and remove out-of-range entries
        IF (LCHECK) THEN
          CALL MC59F(NC,NR,NE,IRN,NC+1,IP,LAA,A,LIW,IW,IDUP,
     +                IOUT,IUP,KNE,ICNTL6,INFO)
C Return if IP not monotonic.
          IF (INFO(1).EQ.-9) GO TO 40
C Return if ALL entries out-of-range.
          IF (KNE.EQ.0) GO TO 50
        ELSE
           KNE = NE
        END IF

C  Order by rows within each column
        CALL MC59D(NC,KNE,IRN,IP,LAA,A)

      END IF

      INFO(3) = IDUP
      INFO(4) = IOUT
      INFO(5) = JOUT
      INFO(6) = KNE
      INFO(7) = IUP

C Set warning flag if out-of-range /duplicates found
      IF (IDUP.GT.0) INFO(1) = INFO(1) + 1
      IF (IOUT.GT.0) INFO(1) = INFO(1) + 2
      IF (JOUT.GT.0) INFO(1) = INFO(1) + 4
      IF (INFO(1).GT.0 .AND. MP.GT.0) THEN
        WRITE (MP,FMT=9080) INFO(1)
        IF (IOUT.GT.0) WRITE (MP,FMT=9090) IOUT
        IF (JOUT.GT.0) WRITE (MP,FMT=9110) JOUT
        IF (IDUP.GT.0) WRITE (MP,FMT=9100) IDUP
        IF (IUP.GT.0)  WRITE (MP,FMT=9130) IUP
      END IF
      GO TO 70

   40 INFO(3) = IDUP
      INFO(4) = IOUT
      INFO(7) = IUP
      IF (LP.GT.0) THEN
        WRITE (LP,FMT=9000) INFO(1)
        WRITE (LP,FMT=9140)
      END IF
      GO TO 70

   50 INFO(1) = -10
      INFO(4) = IOUT
      INFO(5) = JOUT
      INFO(2) = IOUT + JOUT
      IF (LP.GT.0) THEN
        WRITE (LP,FMT=9000) INFO(1)
        WRITE (LP,FMT=9120)
      END IF
   70 RETURN

 9000 FORMAT (/,' *** Error return from MC59A *** INFO(1) = ',I3)
 9010 FORMAT (1X,'ICNTL(2) = ',I2,' is out of range')
 9020 FORMAT (1X,'NC = ',I6,' is out of range')
 9030 FORMAT (1X,'NR = ',I6,' is out of range')
 9035 FORMAT (1X,'Symmetric case. NC = ',I6,' but NR = ',I6)
 9040 FORMAT (1X,'NE = ',I10,' is out of range')
 9050 FORMAT (1X,'Increase LJCN from ',I10,' to at least ',I10)
 9060 FORMAT (1X,'Increase LA from ',I10,' to at least ',I10)
 9065 FORMAT (1X,'Increase LIP from ',I8,' to at least ',I10)
 9070 FORMAT (1X,'Increase LIW from ',I8,' to at least ',I10)
 9080 FORMAT (/,' *** Warning message from MC59A *** INFO(1) = ',I3)
 9090 FORMAT (1X,I8,' entries in IRN supplied by the user were ',
     +       /,'       out of range and were ignored by the routine')
 9100 FORMAT (1X,I8,' duplicate entries were supplied by the user')
 9110 FORMAT (1X,I8,' entries in JCN supplied by the user were ',
     +       /,'       out of range and were ignored by the routine')
 9120 FORMAT (1X,'All entries out of range')
 9130 FORMAT (1X,I8,' of these entries were in the upper triangular ',
     +       /,'       part of matrix')
 9140 FORMAT (1X,'Entries in IP are not monotonic increasing')
 9150 FORMAT (1X,'ICNTL(6) = ',I2,' is out of range')
      END
C***********************************************************************
      SUBROUTINE MC59B(LCHECK,PART,NC,NR,NE,IRN,JCN,LA,A,IP,IW,IOUT,
     +                  JOUT,KNE)
C
C   To sort a sparse matrix from arbitrary order to
C   column order, unordered within each column. Optionally
C   checks for out-of-range entries in IRN,JCN.
C
C LCHECK - logical variable. Intent(IN). If true, check
C          for out-of-range indices.
C PART -   integer variable. Intent(IN)
C PART =  0 : unsymmetric case, whole matrix wanted
C PART =  1 : symmetric case, lower triangular part of matrix wanted
C             (ie IRN(K) .ge. JCN(K) on exit)
C PART = -1 : symmetric case, upper triangular part of matrix wanted
C             (ie IRN(K) .le. JCN(K) on exit)
C   NC - integer variable. Intent(IN)
C      - on entry must be set to the number of columns in the matrix
C   NR - integer variable. Intent(IN)
C      - on entry must be set to the number of rows in the matrix
C   NE - integer variable. Intent(IN)
C      - on entry, must be set to the number of nonzeros in the matrix
C  IRN - integer array of length NE. Intent(INOUT)
C      - on entry set to contain the row indices of the nonzeros
C        in arbitrary order.
C      - on exit, the entries in IRN are reordered so that the row
C        indices for column 1 precede those for column 2 and so on,
C        but the order within columns is arbitrary.
C  JCN - integer array of length NE. Intent(INOUT)
C      - on entry set to contain the column indices of the nonzeros
C      - JCN(K) must be the column index of
C        the entry in IRN(K)
C      - on exit, JCN(K) is the column index for the entry with
C        row index IRN(K) (K=1,...,NE).
C  LA  - integer variable which defines the length of the array A.
C        Intent(IN)
C   A  - real (double precision/complex/complex*16) array of length LA
C        Intent(INOUT)
C      - if LA > 1, the array must be of length NE, and A(K)
C        must be set to the value of the entry in (IRN(K), JCN(K));
C        on exit A is reordered in the same way as IRN
C      - if LA = 1, the array is not accessed
C  IP  - integer array of length NC+1. Intent(INOUT)
C      - not set on entry
C      - on exit, IP(J) contains the position in IRN (and A) of the
C        first entry in column J (J=1,...,NC)
C      - IP(NC+1) is set to NE+1
C  IW  - integer array of length NC+1.  Intent(INOUT)
C      - the array is used as workspace
C      - on exit IW(I) = IP(I) (so IW(I) points to the beginning
C        of column I).
C IOUT - integer variable. Intent(OUT). On exit, holds number
C        of entries in IRN found to be out-of-range
C JOUT - integer variable. Intent(OUT). On exit, holds number
C        of entries in JCN found to be out-of-range
C  KNE - integer variable. Intent(OUT). On exit, holds number
C        of entries in matrix after removal of out-of-range entries.
C        If no data checking, KNE = NE.

C     .. Scalar Arguments ..
      INTEGER LA,NC,NE,NR,IOUT,JOUT,KNE,PART
      LOGICAL LCHECK
C     ..
C     .. Array Arguments ..
      REAL A(LA)
      INTEGER IP(NC+1),IRN(NE),IW(NC+1),JCN(NE)
C     ..
C     .. Local Scalars ..
      REAL ACE,ACEP
      INTEGER I,ICE,ICEP,J,JCE,JCEP,K,L,LOC
C     ..
C     .. Executable Statements ..

C Initialise IW
      DO 10 J = 1,NC + 1
        IW(J) = 0
   10 CONTINUE

      KNE = 0
      IOUT = 0
      JOUT = 0
C Count the number of entries in each column and store in IW.
C We also allow checks for out-of-range indices
      IF (LCHECK) THEN
C Check data.
C Treat case of pattern only separately.
        IF (LA.GT.1) THEN
          IF (PART.EQ.0) THEN
C Unsymmetric
            DO 20 K = 1,NE
              I = IRN(K)
              J = JCN(K)
              IF (I.GT.NR .OR. I.LT.1) THEN
                IOUT = IOUT + 1
C IRN out-of-range. Is JCN also out-of-range?
                IF (J.GT.NC .OR. J.LT.1)  JOUT = JOUT + 1
              ELSE IF (J.GT.NC .OR. J.LT.1) THEN
                JOUT = JOUT + 1
              ELSE
                KNE = KNE + 1
                IRN(KNE) = I
                JCN(KNE) = J
                A(KNE) = A(K)
                IW(J) = IW(J) + 1
              END IF
   20       CONTINUE
          ELSE IF (PART.EQ.1) THEN
C Symmetric, lower triangle
            DO 21 K = 1,NE
              I = IRN(K)
              J = JCN(K)
              IF (I.GT.NR .OR. I.LT.1) THEN
                IOUT = IOUT + 1
C IRN out-of-range. Is JCN also out-of-range?
                IF (J.GT.NC .OR. J.LT.1)  JOUT = JOUT + 1
              ELSE IF (J.GT.NC .OR. J.LT.1) THEN
                JOUT = JOUT + 1
              ELSE
                KNE = KNE + 1
C Lower triangle ... swap if necessary
                IF (I.LT.J) THEN
                  IRN(KNE) = J
                  JCN(KNE) = I
                  IW(I) = IW(I) + 1
                ELSE
                  IRN(KNE) = I
                  JCN(KNE) = J
                  IW(J) = IW(J) + 1
                END IF
                A(KNE) = A(K)
              END IF
   21       CONTINUE
          ELSE IF (PART.EQ.-1) THEN
C Symmetric, upper triangle
            DO 22 K = 1,NE
              I = IRN(K)
              J = JCN(K)
              IF (I.GT.NR .OR. I.LT.1) THEN
                IOUT = IOUT + 1
C IRN out-of-range. Is JCN also out-of-range?
                IF (J.GT.NC .OR. J.LT.1)  JOUT = JOUT + 1
              ELSE IF (J.GT.NC .OR. J.LT.1) THEN
                JOUT = JOUT + 1
              ELSE
                KNE = KNE + 1
C Upper triangle ... swap if necessary
                IF (I.GT.J) THEN
                  IRN(KNE) = J
                  JCN(KNE) = I
                  IW(I) = IW(I) + 1
                ELSE
                  IRN(KNE) = I
                  JCN(KNE) = J
                  IW(J) = IW(J) + 1
                END IF
                A(KNE) = A(K)
              END IF
   22       CONTINUE
          END IF
        ELSE
C Pattern only
          IF (PART.EQ.0) THEN
            DO 25 K = 1,NE
              I = IRN(K)
              J = JCN(K)
              IF (I.GT.NR .OR. I.LT.1) THEN
                IOUT = IOUT + 1
                IF (J.GT.NC .OR. J.LT.1)  JOUT = JOUT + 1
              ELSE IF (J.GT.NC .OR. J.LT.1) THEN
                JOUT = JOUT + 1
              ELSE
                KNE = KNE + 1
                IRN(KNE) = I
                JCN(KNE) = J
                IW(J) = IW(J) + 1
              END IF
   25       CONTINUE
          ELSE IF (PART.EQ.1) THEN
            DO 26 K = 1,NE
              I = IRN(K)
              J = JCN(K)
              IF (I.GT.NR .OR. I.LT.1) THEN
                IOUT = IOUT + 1
                IF (J.GT.NC .OR. J.LT.1)  JOUT = JOUT + 1
              ELSE IF (J.GT.NC .OR. J.LT.1) THEN
                JOUT = JOUT + 1
              ELSE
                KNE = KNE + 1
C Lower triangle ... swap if necessary
                IF (I.LT.J) THEN
                  IRN(KNE) = J
                  JCN(KNE) = I
                  IW(I) = IW(I) + 1
                ELSE
                  IRN(KNE) = I
                  JCN(KNE) = J
                  IW(J) = IW(J) + 1
                END IF
              END IF
   26       CONTINUE
          ELSE IF (PART.EQ.-1) THEN
            DO 27 K = 1,NE
              I = IRN(K)
              J = JCN(K)
              IF (I.GT.NR .OR. I.LT.1) THEN
                IOUT = IOUT + 1
                IF (J.GT.NC .OR. J.LT.1)  JOUT = JOUT + 1
              ELSE IF (J.GT.NC .OR. J.LT.1) THEN
                JOUT = JOUT + 1
              ELSE
                KNE = KNE + 1
C Upper triangle ... swap if necessary
                IF (I.GT.J) THEN
                  IRN(KNE) = J
                  JCN(KNE) = I
                  IW(I) = IW(I) + 1
                ELSE
                  IRN(KNE) = I
                  JCN(KNE) = J
                  IW(J) = IW(J) + 1
                END IF
              END IF
   27       CONTINUE
          END IF
        END IF
C Return if ALL entries out-of-range.
        IF (KNE.EQ.0) GO TO 130

      ELSE

C No checks
        KNE = NE
        IF (PART.EQ.0) THEN
          DO 30 K = 1,NE
            J = JCN(K)
            IW(J) = IW(J) + 1
   30     CONTINUE
        ELSE IF (PART.EQ.1) THEN
          DO 35 K = 1,NE
            I = IRN(K)
            J = JCN(K)
C Lower triangle ... swap if necessary
            IF (I.LT.J) THEN
               IRN(K) = J
               JCN(K) = I
               IW(I) = IW(I) + 1
            ELSE
              IW(J) = IW(J) + 1
            END IF
   35     CONTINUE
        ELSE IF (PART.EQ.-1) THEN
          DO 36 K = 1,NE
            I = IRN(K)
            J = JCN(K)
C Upper triangle ... swap if necessary
            IF (I.GT.J) THEN
               IRN(K) = J
               JCN(K) = I
               IW(I) = IW(I) + 1
            ELSE
              IW(J) = IW(J) + 1
            END IF
   36     CONTINUE
        END IF
      END IF

C KNE is now the number of nonzero entries in matrix.

C Put into IP and IW the positions where each column
C would begin in a compressed collection with the columns
C in natural order.

      IP(1) = 1
      DO 37 J = 2,NC + 1
        IP(J) = IW(J-1) + IP(J-1)
        IW(J-1) = IP(J-1)
   37 CONTINUE

C Reorder the elements into column order.
C Fill in each column from the front, and as a new entry is placed
C in column K increase the pointer IW(K) by one.

      IF (LA.EQ.1) THEN
C Pattern only
        DO 70 L = 1,NC
          DO 60 K = IW(L),IP(L+1) - 1
            ICE = IRN(K)
            JCE = JCN(K)
            DO 40 J = 1,NE
              IF (JCE.EQ.L) GO TO 50
              LOC = IW(JCE)
              JCEP = JCN(LOC)
              ICEP = IRN(LOC)
              IW(JCE) = LOC + 1
              JCN(LOC) = JCE
              IRN(LOC) = ICE
              JCE = JCEP
              ICE = ICEP
   40       CONTINUE
   50       JCN(K) = JCE
            IRN(K) = ICE
   60     CONTINUE
   70   CONTINUE
      ELSE

        DO 120 L = 1,NC
          DO 110 K = IW(L),IP(L+1) - 1
            ICE = IRN(K)
            JCE = JCN(K)
            ACE = A(K)
            DO 90 J = 1,NE
              IF (JCE.EQ.L) GO TO 100
              LOC = IW(JCE)
              JCEP = JCN(LOC)
              ICEP = IRN(LOC)
              IW(JCE) = LOC + 1
              JCN(LOC) = JCE
              IRN(LOC) = ICE
              JCE = JCEP
              ICE = ICEP
              ACEP = A(LOC)
              A(LOC) = ACE
              ACE = ACEP
   90       CONTINUE
  100       JCN(K) = JCE
            IRN(K) = ICE
            A(K) = ACE
  110     CONTINUE
  120   CONTINUE
      END IF

  130 CONTINUE

      RETURN
      END
C
C**********************************************************
      SUBROUTINE MC59C(NC,NR,NE,IRN,JCN,LA,A,IP,IW)
C
C   To sort a sparse matrix stored by rows,
C   unordered within each row, to ordering by columns, with
C   ordering by rows within each column.
C
C   NC - integer variable. Intent(IN)
C      - on entry must be set to the number of columns in the matrix
C   NR - integer variable. Intent(IN)
C      - on entry must be set to the number of rows in the matrix
C  NE - integer variable. Intent(IN)
C      - on entry, must be set to the number of nonzeros in the matrix
C  IRN - integer array of length NE. Intent(OUT).
C      - not set on entry.
C      - on exit,  IRN holds row indices with the row
C        indices for column 1 preceding those for column 2 and so on,
C        with ordering by rows within each column.
C  JCN - integer array of length NE. Intent(INOUT)
C      - on entry set to contain the column indices of the nonzeros
C        with indices for column 1 preceding those for column 2
C        and so on, with the order within columns is arbitrary.
C      - on exit, contents destroyed.
C  LA  - integer variable which defines the length of the array A.
C        Intent(IN)
C   A  - real (double precision/complex/complex*16) array of length LA
C        Intent(INOUT)
C      - if LA > 1, the array must be of length NE, and A(K)
C        must be set to the value of the entry in JCN(K);
C        on exit A, A(K) holds the value of the entry in IRN(K).
C      - if LA = 1, the array is not accessed
C  IP  - integer array of length NC+1. Intent(INOUT)
C      - not set on entry
C      - on exit, IP(J) contains the position in IRN (and A) of the
C        first entry in column J (J=1,...,NC)
C      - IP(NC+1) is set to NE+1
C  IW  - integer array of length NR+1.  Intent(IN)
C      - on entry, must be set on entry so that IW(J) points to the
C        position in JCN of the first entry in row J, J=1,...,NR, and
C        IW(NR+1) must be set to NE+1
C
C     .. Scalar Arguments ..
      INTEGER LA,NC,NE,NR
C     ..
C     .. Array Arguments ..
      REAL A(LA)
      INTEGER IP(NC+1),IRN(NE),IW(NR+1),JCN(NE)
C     ..
C     .. Local Scalars ..
      REAL ACE,ACEP
      INTEGER I,ICE,ICEP,J,J1,J2,K,L,LOC,LOCP
C     ..
C     .. Executable Statements ..

C  Count the number of entries in each column

      DO 10 J = 1,NC
        IP(J) = 0
   10 CONTINUE

      IF (LA.GT.1) THEN

        DO 20 K = 1,NE
          I = JCN(K)
          IP(I) = IP(I) + 1
          IRN(K) = JCN(K)
   20   CONTINUE
        IP(NC+1) = NE + 1

C  Set IP so that IP(I) points to the first entry in column I+1

        IP(1) = IP(1) + 1
        DO 30 J = 2,NC
          IP(J) = IP(J) + IP(J-1)
   30   CONTINUE

        DO 50 I = NR,1,-1
          J1 = IW(I)
          J2 = IW(I+1) - 1
          DO 40 J = J1,J2
            K = IRN(J)
            L = IP(K) - 1
            JCN(J) = L
            IRN(J) = I
            IP(K) = L
   40     CONTINUE
   50   CONTINUE
        IP(NC+1) = NE + 1
        DO 70 J = 1,NE
          LOC = JCN(J)
          IF (LOC.EQ.0) GO TO 70
          ICE = IRN(J)
          ACE = A(J)
          JCN(J) = 0
          DO 60 K = 1,NE
            LOCP = JCN(LOC)
            ICEP = IRN(LOC)
            ACEP = A(LOC)
            JCN(LOC) = 0
            IRN(LOC) = ICE
            A(LOC) = ACE
            IF (LOCP.EQ.0) GO TO 70
            ICE = ICEP
            ACE = ACEP
            LOC = LOCP
   60     CONTINUE
   70   CONTINUE
      ELSE

C Pattern only

C  Count the number of entries in each column

        DO 90 K = 1,NE
          I = JCN(K)
          IP(I) = IP(I) + 1
   90   CONTINUE
        IP(NC+1) = NE + 1

C  Set IP so that IP(I) points to the first entry in column I+1

        IP(1) = IP(1) + 1
        DO 100 J = 2,NC
          IP(J) = IP(J) + IP(J-1)
  100   CONTINUE

        DO 120 I = NR,1,-1
          J1 = IW(I)
          J2 = IW(I+1) - 1
          DO 110 J = J1,J2
            K = JCN(J)
            L = IP(K) - 1
            IRN(L) = I
            IP(K) = L
  110     CONTINUE
  120   CONTINUE

      END IF

      RETURN
      END

C**********************************************************

      SUBROUTINE MC59D(NC,NE,IRN,IP,LA,A)
C
C To sort from arbitrary order within each column to order
C by increasing row index. Note: this is taken from MC20B/BD.
C
C   NC - integer variable. Intent(IN)
C      - on entry must be set to the number of columns in the matrix
C   NE - integer variable. Intent(IN)
C      - on entry, must be set to the number of nonzeros in the matrix
C  IRN - integer array of length NE. Intent(INOUT)
C      - on entry set to contain the row indices of the nonzeros
C        ordered so that the row
C        indices for column 1 precede those for column 2 and so on,
C        but the order within columns is arbitrary.
C        On exit, the order within each column is by increasing
C        row indices.
C   LA - integer variable which defines the length of the array A.
C        Intent(IN)
C    A - real (double precision/complex/complex*16) array of length LA
C        Intent(INOUT)
C      - if LA > 1, the array must be of length NE, and A(K)
C        must be set to the value of the entry in IRN(K);
C        on exit A is reordered in the same way as IRN
C      - if LA = 1, the array is not accessed
C  IP  - integer array of length NC. Intent(IN)
C      - on entry, IP(J) contains the position in IRN (and A) of the
C        first entry in column J (J=1,...,NC)
C     . .
C     .. Scalar Arguments ..
      INTEGER LA,NC,NE
C     ..
C     .. Array Arguments ..
      REAL A(LA)
      INTEGER IRN(NE),IP(NC)
C     ..
C     .. Local Scalars ..
      REAL ACE
      INTEGER ICE,IK,J,JJ,K,KDUMMY,KLO,KMAX,KOR
C     ..
C     .. Intrinsic Functions ..
      INTRINSIC ABS
C     ..
C     .. Executable Statements ..

C Jump if pattern only.
      IF (LA.GT.1) THEN
        KMAX = NE
        DO 50 JJ = 1,NC
          J = NC + 1 - JJ
          KLO = IP(J) + 1
          IF (KLO.GT.KMAX) GO TO 40
          KOR = KMAX
          DO 30 KDUMMY = KLO,KMAX
C Items KOR, KOR+1, .... ,KMAX are in order
            ACE = A(KOR-1)
            ICE = IRN(KOR-1)
            DO 10 K = KOR,KMAX
              IK = IRN(K)
              IF (ABS(ICE).LE.ABS(IK)) GO TO 20
              IRN(K-1) = IK
              A(K-1) = A(K)
   10       CONTINUE
            K = KMAX + 1
   20       IRN(K-1) = ICE
            A(K-1) = ACE
            KOR = KOR - 1
   30     CONTINUE
C Next column
   40     KMAX = KLO - 2
   50   CONTINUE
      ELSE

C Pattern only.
        KMAX = NE
        DO 150 JJ = 1,NC
          J = NC + 1 - JJ
          KLO = IP(J) + 1
          IF (KLO.GT.KMAX) GO TO 140
          KOR = KMAX
          DO 130 KDUMMY = KLO,KMAX
C Items KOR, KOR+1, .... ,KMAX are in order
            ICE = IRN(KOR-1)
            DO 110 K = KOR,KMAX
              IK = IRN(K)
              IF (ABS(ICE).LE.ABS(IK)) GO TO 120
              IRN(K-1) = IK
  110       CONTINUE
            K = KMAX + 1
  120       IRN(K-1) = ICE
            KOR = KOR - 1
  130     CONTINUE
C Next column
  140     KMAX = KLO - 2
  150   CONTINUE
      END IF
      END
C***********************************************************************

      SUBROUTINE MC59E(NC,NR,NE,IRN,LIP,IP,LA,A,IW,IDUP,KNE,ICNTL6)

C Checks IRN for duplicate entries.
C On exit, IDUP holds number of duplicates found and KNE is number
C of entries in matrix after removal of duplicates
C     . .
C     .. Scalar Arguments ..
      INTEGER ICNTL6,IDUP,KNE,LIP,LA,NC,NR,NE
C     ..
C     .. Array Arguments ..
      REAL A(LA)
      INTEGER IRN(NE),IP(LIP),IW(NR)
C     ..
C     .. Local Scalars ..
      INTEGER I,J,K,KSTART,KSTOP,NZJ

      IDUP = 0
      KNE = 0
C Initialise IW
      DO 10 I = 1,NR
        IW(I) = 0
   10 CONTINUE

      KSTART = IP(1)
      IF (LA.GT.1) THEN
C Matrix entries considered
        NZJ = 0
        DO 30 J = 1,NC
          KSTOP = IP(J+1)
          IP(J+1) = IP(J)
          DO 20 K = KSTART,KSTOP - 1
            I = IRN(K)
            IF (IW(I).LE.NZJ) THEN
              KNE = KNE + 1
              IRN(KNE) = I
              A(KNE) = A(K)
              IP(J+1) = IP(J+1) + 1
              IW(I) = KNE
            ELSE
C We have a duplicate in column J
              IDUP = IDUP + 1
C If requested, sum duplicates
              IF (ICNTL6.GE.0) A(IW(I)) = A(IW(I)) + A(K)
            END IF
   20     CONTINUE
          KSTART = KSTOP
          NZJ = KNE
   30   CONTINUE

      ELSE

C Pattern only
        DO 50 J = 1,NC
          KSTOP = IP(J+1)
          IP(J+1) = IP(J)
          DO 40 K = KSTART,KSTOP - 1
            I = IRN(K)
            IF (IW(I).LT.J) THEN
              KNE = KNE + 1
              IRN(KNE) = I
              IP(J+1) = IP(J+1) + 1
              IW(I) = J
            ELSE
C  We have a duplicate in column J
              IDUP = IDUP + 1
            END IF
   40     CONTINUE
          KSTART = KSTOP
   50   CONTINUE
      END IF

      RETURN
      END
C***********************************************************************

      SUBROUTINE MC59F(NC,NR,NE,IRN,LIP,IP,LA,A,LIW,IW,IDUP,IOUT,
     +                  IUP,KNE,ICNTL6,INFO)

C Checks IRN for duplicate and out-of-range entries.
C For symmetric matrix, also checks NO entries lie in upper triangle.
C Also checks IP is monotonic.
C On exit:
C IDUP holds number of duplicates found
C IOUT holds number of out-of-range entries
C For symmetric matrix, IUP holds number of entries in upper
C triangular part.
C KNE holds number of entries in matrix after removal of
C out-of-range and duplicate entries.
C Note: this is similar to MC59E except it also checks IP is
C monotonic and removes out-of-range entries in IRN.
C     . .
C     .. Scalar Arguments ..
      INTEGER ICNTL6,IDUP,IOUT,IUP,KNE,LA,LIP,LIW,NC,NR,NE
C     ..
C     .. Array Arguments ..
      REAL A(LA)
      INTEGER IRN(NE),IP(LIP),IW(LIW),INFO(2)
C     ..
C     .. Local Scalars ..
      INTEGER I,J,K,KSTART,KSTOP,NZJ,LOWER

      IDUP = 0
      IOUT = 0
      IUP = 0
      KNE = 0
C Initialise IW
      DO 10 I = 1,NR
        IW(I) = 0
   10 CONTINUE

      KSTART = IP(1)
      LOWER = 1
      IF (LA.GT.1) THEN
        NZJ = 0
        DO 30 J = 1,NC
C In symmetric case, entries out-of-range if they lie
C in upper triangular part.
          IF (ICNTL6.NE.0) LOWER = J
          KSTOP = IP(J+1)
          IF (KSTART.GT.KSTOP) THEN
            INFO(1) = -9
            INFO(2) = J
            RETURN
          END IF
          IP(J+1) = IP(J)
          DO 20 K = KSTART,KSTOP - 1
            I = IRN(K)
C Check for out-of-range
            IF (I.GT.NR .OR. I.LT.LOWER) THEN
              IOUT = IOUT + 1
C In symmetric case, check if entry is out-of-range because
C it lies in upper triangular part.
              IF (ICNTL6.NE.0 .AND. I.LT.J) IUP = IUP + 1
            ELSE IF (IW(I).LE.NZJ) THEN
              KNE = KNE + 1
              IRN(KNE) = I
              A(KNE) = A(K)
              IP(J+1) = IP(J+1) + 1
              IW(I) = KNE
            ELSE
C  We have a duplicate in column J
              IDUP = IDUP + 1
C If requested, sum duplicates
              IF (ICNTL6.GE.0) A(IW(I)) = A(IW(I)) + A(K)
            END IF
   20     CONTINUE
          KSTART = KSTOP
          NZJ = KNE
   30   CONTINUE

      ELSE

C Pattern only
        DO 50 J = 1,NC
C In symmetric case, entries out-of-range if lie
C in upper triangular part.
          IF (ICNTL6.NE.0) LOWER = J
          KSTOP = IP(J+1)
          IF (KSTART.GT.KSTOP) THEN
            INFO(1) = -9
            INFO(2) = J
            RETURN
          END IF
          IP(J+1) = IP(J)
          DO  40 K = KSTART,KSTOP - 1
            I = IRN(K)
C Check for out-of-range
            IF (I.GT.NR .OR. I.LT.LOWER) THEN
              IOUT = IOUT + 1
              IF (ICNTL6.NE.0 .AND. I.GT.1) IUP = IUP + 1
            ELSE IF (IW(I).LT.J) THEN
              KNE = KNE + 1
              IRN(KNE) = I
              IP(J+1) = IP(J+1) + 1
              IW(I) = J
            ELSE
C  We have a duplicate in column J
              IDUP = IDUP + 1
            END IF
   40     CONTINUE
          KSTART = KSTOP
   50   CONTINUE
      END IF

      RETURN
      END

! COPYRIGHT (c) 1979 AEA Technology and
! Council for the Central Laboratory of the Research Councils
!
! Version 1.0.1
! See ChangeLog for version history
!
      REAL FUNCTION FA14A(IX,I)
C         NEARLY PORTABLE RANDOM NUMBER GENERATOR USING THE RECURSION
C                       IX=IX*A MOD P
C
C    WHERE A=7**5
C    AND P=2**31-1.
C
C         THIS FUNCTION DOES NOT ADHERE TO THE ANSI STANDARD 1966 IN
C    TWO RESPECTS:
C      1) IT ASSUMES AN INTEGER WORD LENGTH OF AT LEAST 32 BITS (I.E.
C    INTEGERS WHICH LIE IN THE RANGE 1-2**31 TO 2**31-1 INCLUSIVE MUST
C    BE REPRESENTABLE);
C      2) IT ASSUMES THAT A POSITIVE INTEGER LESS THAN 2**16 MAY BE
C    FLOATED WITHOUT LOSS OF DIGITS.
C
C         THIS CODE IS BASED ON CODE PUBLISHED BY LINUS SCHRAGE IN
C    T.O.M.S. VOL.5 NO.2 JUNE 1979 (PP 132-138)
C
C
C
C       THE FUNCTION IS USED AS FOLLOWS:
C
C                      R=FA14A(IX,I)
C
C       WHERE IX IS THE GENERATOR WORD
C             I IS AN INTEGER SET BY THE USER.
C
C
C       THE VALUE RETURNED BY FA14A/AD WILL LIE IN THE RANGE
C                (0.,1.)  IF I IS NON-NEGATIVE
C                (-1.,1.) IF I IS NEGATIVE.
C
C       THE METHOD EMPLOYED IS A MULTIPLICATIVE CONGRUENTIAL
C   ONE USING A MULTIPLIER OF 7**5 AND TAKING THE MODULO TO
C   2**31-1, I.E. THE GENERATOR NUMBER , G = IX, IS UPDATED ON
C   EACH CALL TO THE VALUE
C
C                  5          31
C               G*7  MODULO (2  -1)
C
C       THE RESULT RETURNED IS CALCULATED AS A DOUBLE
C  PRECISION NUMBER HAVING THE VALUE
C
C                      31
C                  G/(2   -1)    IF THE ARGUMENT IS
C                                NON-NEGATIVE
C           OR
C                      31
C                2*G/(2   -1)-1  IF THE ARGUMENT IS NEGATIVE
C
C
C 7**5, 2**15, 2**16, 2**31-1
C     .. Parameters ..
      INTEGER A,B15,B16,P
      PARAMETER (A=16807,B15=32768,B16=65536,P=2147483647)
C     ..
C     .. Scalar Arguments ..
      INTEGER IX,I
C     ..
C     .. Local Scalars ..
      DOUBLE PRECISION X
      INTEGER FHI,K,LEFTLO,XALO,XHI
C     ..
C     .. Intrinsic Functions ..
      INTRINSIC FLOAT
C     ..
C     .. Executable Statements ..
C
C GET 15 HI ORDER BITS OF IX
      XHI = IX/B16
C GET 16 LO BITS OF IX AND FORM LO PRODUCT
      XALO = (IX-XHI*B16)*A
C GET 15 HI ORDER BITS OF LO PRODUCT
      LEFTLO = XALO/B16
C     FORM THE 31 HIGHEST BITS OF FULL PRODUCT
      FHI = XHI*A + LEFTLO
C GET OVERFLOPAST 31ST BIT OF FULL PRODUCT
      K = FHI/B15
C ASSEMBLE ALL THE PARTS AND PRESUBTRACT P
C THE PARENTHESES ARE ESSENTIAL
      IX = (((XALO-LEFTLO*B16)-P)+ (FHI-K*B15)*B16) + K
C ADD P BACK IN IF NECCESSARY
      IF (IX.LT.0) IX = IX + P
C MULTIPLY BY 1/(2**31-1)
      XHI = IX/B16
      X = (FLOAT(XHI)*65536.0D0) + FLOAT(IX-XHI*B16)
      IF (I.GE.0) FA14A = X*4.6566128752457969241D-10
      IF (I.LT.0) FA14A = X*9.3132257504915938482D-10 - 1.0D0
      RETURN

      END
      SUBROUTINE FA14B(IX,MAX,NRAND)
C         NEARLY PORTABLE RANDOM NUMBER GENERATOR USING THE RECURSION
C                       IX=IX*A MOD P
C
C    WHERE A=7**5
C    AND P=2**31-1.
C
C         THIS SUBROUTINE DOES NOT ADHERE TO THE ANSI STANDARD 1966
C    IN ONE RESPECT:
C         IT ASSUMES AN INTEGER WORD LENGTH OF AT LEAST 32 BITS (I.E.
C    INTEGERS WHICH LIE IN THE RANGE 1-2**31 TO 2**31-1 INCLUSIVE MUST
C    BE REPRESENTABLE).
C
C         THIS CODE IS BASED ON CODE PUBLISHED BY LINUS SCHRAGE IN
C    T.O.M.S. VOL.5 NO.2 JUNE 1979 (PP 132-138)
C
C
C       THE FUNCTION IS USED AS FOLLOWS:
C
C                  CALL FA14B(IX,MAX,NRAND)
C
C       WHERE IX    IS THE GENERATOR WORD
C             MAX   IS AN INTEGER SET BY THE USER AND
C             NRAND IS AN INTEGER SET BY FA14B/BD.
C
C
C       THE VALUE OF NRAND RETURNED BY FA14B/BD WILL LIE IN THE
C   RANGE
C                        (1,MAX)
C
C       THE METHOD EMPLOYED IS A MULTIPLICATIVE CONGRUENTIAL
C   ONE USING A MULTIPLIER OF 7**5 AND TAKING THE MODULO TO
C   2**31-1, I.E. THE GENERATOR NUMBER , G = IX, IS UPDATED ON
C   EACH CALL TO THE VALUE
C
C                  5          31
C               G*7  MODULO (2  -1)
C
C       THE RESULT RETURNED IS AN INTEGER NUMBER
C   HAVING THE VALUE
C
C                        31
C   INT. PART( (MAX*G)/(2   -1) ) + 1
C
C
C 7**5, 2**15, 2**16, 2**31-1
C 2**30,  2**30-1
C     .. Parameters ..
      INTEGER A,B15,B16,P
      PARAMETER (A=16807,B15=32768,B16=65536,P=2147483647)
      INTEGER B30,Q
      PARAMETER (B30=1073741824,Q=1073741823)
C     ..
C     .. Scalar Arguments ..
      INTEGER IX,MAX,NRAND
C     ..
C     .. Local Scalars ..
      INTEGER BE1,BE2,C,D,F,FHI,G,K,LEFTLO,MHI,MLO,MU,NU,XALO,XHI,XLO
C     ..
C     .. Executable Statements ..
C
C GET 15 HI ORDER BITS OF IX
      XHI = IX/B16
C GET 16 LO BITS OF IX AND FORM LO PRODUCT
      XALO = (IX-XHI*B16)*A
C GET 15 HI ORDER BITS OF LO PRODUCT
      LEFTLO = XALO/B16
C     FORM THE 31 HIGHEST BITS OF FULL PRODUCT
      FHI = XHI*A + LEFTLO
C GET OVERFLOPAST 31ST BIT OF FULL PRODUCT
      K = FHI/B15
C ASSEMBLE ALL THE PARTS AND PRESUBTRACT P
C THE PARENTHESES ARE ESSENTIAL
      IX = (((XALO-LEFTLO*B16)-P)+ (FHI-K*B15)*B16) + K
C ADD P BACK IN IF NECCESSARY
      IF (IX.LT.0) IX = IX + P
C MULTIPLY BY MAX AND DIVIDE BY 2**31-1 IN INTEGER ARITHMETIC
C SPLIT IX AND MAX INTO HI AND LO PARTS
      XHI = IX/B15
      XLO = IX - B15*XHI
      MHI = MAX/B15
      MLO = MAX - B15*MHI
C CALCULATE INTERMEDIATE PRODUCT AND SPLIT INTO HI AND LO PARTS
C PRESUBTRACT P
      F = (XHI*MLO-P) + XLO*MHI
C F IS > 0 IF INTERMEDIATE PRODUCT WOULD HAVE OVERFLOWED
      IF (F.GT.0) GO TO 1
      F = F + P
      BE1 = F/B15
      BE2 = F - BE1*B15
      GO TO 2

    1 F = F - 1
      BE1 = F/B15
      BE2 = F - BE1*B15
      BE1 = BE1 + B16
C FORM PRODUCT OF LO PARTS AND ADD IN LO PART OF INTERMEDIATE PRODUCT
C TO GET LO PART OF COMPLETE PRODUCT
    2 G = B15*BE2 + XLO*MLO
C REPRESENT LO PART OF FULL PRODUCT IN BASE 2**30
      D = G/B30
      C = XHI/2
C CALCULATE FULL PRODUCT DIVIDED BY 2**30
      F = ((2* (C*MHI-Q)-1)+MHI* (XHI-2*C)) + D + BE1
C GET FULL PRODUCT DIVIDED IN BASE 2**31
      IF (F.GT.0) GO TO 3
      F = F + P
      NU = F/2
      MU = F - NU*2
      GO TO 4

    3 F = F - 1
      NU = F/2
      MU = F - 2*NU
      NU = NU + B30
C CALCULATE REMAINDER OF PRODUCT DIVIDED BY 2**31
    4 F = (B30*MU-P) + NU + (G-B30*D)
      NRAND = NU + 1
C  ADD ONE IF REMAINDER IS NOT < 2**31-1
      IF (F.GE.0) NRAND = NRAND + 1
      RETURN

      END
      SUBROUTINE FA14C(IX,IGEN)
C        FA14C IS A SUBROUTINE USED IN CONJUNCTION WITH FA14A OR
C   FA14B. IT PROVIDES THE USER WITH THE FACILITY OF SAVING THE
C   CURRENT VALUE OF THE GENERATOR NUMBER USED BY FA14A AND FA14B.
C
C        USE OF THE ROUTINE IS AS FOLLOWS:
C
C                       CALL FA14C(IX,IGEN)
C
C     WHERE IX   IS THE GENERATOR WORD
C           IGEN IS AN INTEGER WHICH IS SET BY FA14C/CD TO THE CURRENT
C                VALUE OF THE GENERATOR.
C
C
C     .. Scalar Arguments ..
      INTEGER IX,IGEN
C     ..
C     .. Executable Statements ..
      IGEN = IX
      RETURN

      END
      SUBROUTINE FA14D(IX,IGEN)
C        FA14D IS A SUBROUTINE USED IN CONJUNCTION WITH FA14A OR
C   FA14B. IT PROVIDES THE USER WITH THE FACILITY OF SETTING THE
C   CURRENT VALUE OF THE GENERATOR NUMBER USED BY FA14A AND FA14B.
C
C        USE OF THE ROUTINE IS AS FOLLOWS:
C
C                       CALL FA14D(IX,IGEN)
C
C    WHERE IX   IS THE GENERATOR WORD
C          IGEN IS AN INTEGER, SET BY THE USER TO THE VALUE TO WHICH
C               THE GENERATOR IS TO BE SET. IT IS RECOMMENDED THAT THIS
C               VALUE BE OBTAINED BY A PREVIOUS CALL TO FA14C/CD.
C
C     .. Scalar Arguments ..
      INTEGER IX,IGEN
C     ..
C     .. Executable Statements ..
      IX = IGEN
      RETURN

      END
      SUBROUTINE FA14I(IX)
C        FA14I IS A SUBROUTINE USED TO INITIALIZE THE GENERATOR WORD USE
C   BY FA14A AND FA14B. IT MUST BE CALLED FIRST BEFORE ANY OF THE OTHER
C   ENTRIES ARE CALLED.
C
C        USE OF THE ROUTINE IS AS FOLLOWS:
C
C                       CALL FA14I(IX)
C
C    WHERE IX   IS THE GENERATOR WORD
C
C     .. Scalar Arguments ..
      INTEGER IX
C     ..
C     .. Executable Statements ..
      IX = 1
      RETURN

      END
! COPYRIGHT (c) 1975 AEA Technology and
! Council for the Central Laboratory of the Research Councils
!
! Version 1.3.0
! See ChangeLog for version history.
!
      SUBROUTINE LA15I(ICNTL,CNTL,KEEP)
C     .. Array Arguments ..
      REAL CNTL(3)
      INTEGER ICNTL(3)
      INTEGER KEEP(7)
C     ..
C     .. Local Scalars ..
      INTEGER I
C     ..
      ICNTL(1) = 6
      ICNTL(2) = 0
      ICNTL(3) = 0

      CNTL(1) = 0.0
      CNTL(2) = 0.0
      CNTL(3) = 0.0
C
C  Initialize kept data to avoid undefined assignment
      DO 10 I = 1,7
      KEEP(I) = 0
   10 CONTINUE

      RETURN
      END
      SUBROUTINE LA15A(A,IND,NZERO,IA,N,IP,IW,W,G,U,ICNTL,CNTL,KEEP)
C     .. Scalar Arguments ..
      REAL G,U
      INTEGER IA,N,NZERO
C     ..
C     .. Array Arguments ..
      REAL A(IA),W(N)
      INTEGER IND(IA,2),IP(N,2),IW(N,8)
      REAL CNTL(3)
      INTEGER ICNTL(3)
      INTEGER KEEP(7)
C IP(I,1),IP(I,2) POINT TO THE START OF ROW/COL I.
C IW(I,1),IW(I,2) HOLD THE NUMBER OF NON-ZEROS IN ROW/COL I.
C DURING THE MAIN BODY OF THIS SUBROUTINE THE VECTORS IW(.,3),IW(.,5),
C     IW(.,7) ARE USED TO HOLD DOUBLY LINKED LISTS OF ROWS THAT HAVE
C     NOT BEEN PIVOTAL AND HAVE EQUAL NUMBERS OF NON-ZEROS.
C IW(.,4),IW(.,6),IW(.,8) HOLD SIMILAR LISTS FOR THE COLUMNS.
C IW(I,3),IW(I,4) HOLD FIRST ROW/COLUMN TO HAVE I NON-ZEROS
C     OR ZERO IF THERE ARE NONE.
C IW(I,5), IW(I,6) HOLD ROW/COL NUMBER OF ROW/COL PRIOR TO ROW/COL I
C     IN ITS LIST, OR ZERO IF NONE.
C IW(I,7), IW(I,8) HOLD ROW/COL NUMBER OF ROW/COL AFTER ROW/COL I
C     IN ITS LIST, OR ZERO IF NONE.
C FOR ROWS/COLS THAT HAVE BEEN PIVOTAL IW(I,5),IW(I,6) HOLD NEGATION OF
C     POSITION OF ROW/COL I IN THE PIVOTAL ORDERING.
C ICNT59 CONTROLS FOR CALL TO MC59
C INFO59 INFORMATION RETURNED BY MC59 (MOSTLY IGNORED)
C     ..
C     .. Local Scalars ..
      REAL AM,AMAX,AU,EPS,JCOST,KCOST,NZ1
      INTEGER I,IDUMMY,II,IL,IN,IPP,IPV,IR,J,JP,K,K1,K2,KC,KJ,KK,KL,KLC,
     +        KN,KNP,KP,KPC,KPL,KQ,KR,KRL,KS,L,MCP,NC,NZ,NZC
      INTEGER LP,LENL,LENU,NCP,LROW,LCOL,ICNT59(10),INFO59(10)
      REAL SMALL
C     ..
C     .. External Functions ..
      REAL FD15A
      EXTERNAL FD15A
C     ..
C     .. External Subroutines ..
      EXTERNAL LA15E,MC59A
C     ..
C     .. Intrinsic Functions ..
      INTRINSIC ABS,MAX
C     ..
C     .. Executable Statements ..
C
C  Restore kept data
C
      LP    = ICNTL(1)
      SMALL = CNTL(1)
      LENL  = KEEP(1)
      LENU  = KEEP(2)
      NCP   = KEEP(3)
      LROW  = KEEP(4)
      LCOL  = KEEP(5)
C
      EPS = FD15A('E')
C EPS IS THE RELATIVE ACCURACY OF FLOATING-POINT COMPUTATION
      IF (U.GT.1.) U = 1.
      IF (U.LT.EPS) U = EPS
      IF (N.LT.1) GO TO 640
      IF (NZERO.LT.0) GO TO 700
      G = 0.
      DO 20 I = 1,N
        W(I) = 0.
        DO 10 J = 1,5
          IW(I,J) = 0
   10   CONTINUE
   20 CONTINUE
C
C FLUSH OUT SMALL ENTRIES, COUNT ELEMENTS IN ROWS AND COLUMNS
      L = 1
      LENU = NZERO
      DO 50 IDUMMY = 1,NZERO
        IF (L.GT.LENU) GO TO 60
        DO 30 K = L,LENU
          IF (ABS(A(K)).LE.SMALL) GO TO 40
          I = IND(K,1)
          J = IND(K,2)
          G = MAX(ABS(A(K)),G)
          IF (I.LT.1 .OR. I.GT.N) GO TO 650
          IF (J.LT.1 .OR. J.GT.N) GO TO 650
          IW(I,1) = IW(I,1) + 1
          IW(J,2) = IW(J,2) + 1
   30   CONTINUE
        GO TO 60

   40   L = K
        A(L) = A(LENU)
        IND(L,1) = IND(LENU,1)
        IND(L,2) = IND(LENU,2)
        LENU = LENU - 1
   50 CONTINUE
C
   60 LENL = 0
      LROW = LENU
      LCOL = LROW
C MCP IS THE MAXIMUM NUMBER OF COMPRESSES PERMITTED BEFORE AN
C     ERROR RETURN RESULTS.
      MCP = MAX(N/10,20)
      NCP = 0

      IF(LENU.GT.0)THEN
C Reorder by rows. Note that MC59 alters IP(1,2).
        ICNT59(1) = 1
        ICNT59(2) = 0
        ICNT59(3) = 0
        ICNT59(4) = ICNTL(1)
        IF ( ICNT59(4).EQ.0 ) ICNT59(4) = -1
        ICNT59(5) = ICNT59(4)
        ICNT59(6) = 0
        CALL MC59A(ICNT59,N,N,LENU,IND(1,2),IA,IND(1,1),IA,A,N+1,IP,
     +            N+1,IW(1,7),INFO59)
      ELSE
        DO 70 IR = 1,N
          IP(IR,1) = 1
   70   CONTINUE
      END IF

C INITIALIZE IP(I,2) TO POINT JUST BEYOND WHERE THE LAST COMPONENT
C     OF COLUMN I OF A WILL BE STORED.
      K = 1
      DO 80 IR = 1,N
        K = K + IW(IR,2)
        IP(IR,2) = K
   80 CONTINUE
C CHECK FOR DOUBLE ENTRIES WHILE USING THE NEWLY CONSTRUCTED
C     ROW FILE TO CONSTRUCT THE COLUMN FILE. NOTE THAT BY PUTTING
C    THE ENTRIES IN BACKWARDS AND DECREASING IP(J,2) EACH TIME IT
C     IS USED WE AUTOMATICALLY LEAVE IT POINTING TO THE FIRST ELEMENT.
      KL = LENU
      DO 100 II = 1,N
        IR = N + 1 - II
        KP = IP(IR,1)
        DO 90 K = KP,KL
          J = IND(K,2)
          IF (IW(J,5).EQ.IR) GO TO 630
          IW(J,5) = IR
          KR = IP(J,2) - 1
          IP(J,2) = KR
          IND(KR,1) = IR
   90   CONTINUE
        KL = KP - 1
  100 CONTINUE
C
C SET UP LINKED LISTS OF ROWS AND COLS WITH EQUAL NUMBERS OF NON-ZEROS.
      DO 120 L = 1,2
        DO 110 I = 1,N
          NZ = IW(I,L)
          IF(NZ.GT.0)THEN
            IN = IW(NZ,L+2)
            IW(NZ,L+2) = I
            IW(I,L+6) = IN
            IF (IN.NE.0) IW(IN,L+4) = I
          END IF
          IW(I,L+4) = 0
  110   CONTINUE
  120 CONTINUE
C
C
C START OF MAIN ELIMINATION LOOP.
      DO 560 IPV = 1,N
C FIND PIVOT. JCOST IS MARKOWITZ COST OF CHEAPEST PIVOT FOUND SO FAR,
C     WHICH IS IN ROW IPP AND COLUMN JP.
C NZ1 holds NZ-1
        JCOST = N
        JCOST = JCOST*JCOST
C LOOP ON LENGTH OF COLUMN TO BE SEARCHED
        DO 210 NZ = 1,N
          NZ1 = NZ - 1
          IF (JCOST.LE.NZ1**2) GO TO 220
          J = IW(NZ,4)
C SEARCH COLUMNS WITH NZ NON-ZEROS.
          DO 160 IDUMMY = 1,N
            IF (J.LE.0) GO TO 170
            KP = IP(J,2)
            KL = KP + IW(J,2) - 1
            DO 150 K = KP,KL
              I = IND(K,1)
              KCOST = NZ1* (IW(I,1)-1)
              IF (KCOST.GE.JCOST) GO TO 150
              IF (NZ.EQ.1) GO TO 140
C FIND LARGEST ELEMENT IN ROW OF POTENTIAL PIVOT.
              AMAX = 0.0
              K1 = IP(I,1)
              K2 = IW(I,1) + K1 - 1
              DO 130 KK = K1,K2
                AMAX = MAX(AMAX,ABS(A(KK)))
                IF (IND(KK,2).EQ.J) KJ = KK
  130         CONTINUE
C PERFORM STABILITY TEST.
              IF (ABS(A(KJ)).LT.AMAX*U) GO TO 150
  140         JCOST = KCOST
              IPP = I
              JP = J
              IF (JCOST.LE.NZ1**2) GO TO 220
  150       CONTINUE
            J = IW(J,8)
  160     CONTINUE
C SEARCH ROWS WITH NZ NON-ZEROS.
  170     I = IW(NZ,3)
          DO 200 IDUMMY = 1,N
            IF (I.LE.0) GO TO 210
            AMAX = 0.0
            KP = IP(I,1)
            KL = KP + IW(I,1) - 1
C FIND LARGEST ELEMENT IN THE ROW
            DO 180 K = KP,KL
              AMAX = MAX(ABS(A(K)),AMAX)
  180       CONTINUE
            AU = AMAX*U
            DO 190 K = KP,KL
C PERFORM STABILITY TEST.
              IF (ABS(A(K)).LT.AU) GO TO 190
              J = IND(K,2)
              KCOST = NZ1* (IW(J,2)-1)
              IF (KCOST.GE.JCOST) GO TO 190
              JCOST = KCOST
              IPP = I
              JP = J
              IF (JCOST.LE.NZ1**2) GO TO 220
  190       CONTINUE
            I = IW(I,7)
  200     CONTINUE
  210   CONTINUE
        GO TO 680
C
C PIVOT FOUND.
C REMOVE ROWS AND COLUMNS INVOLVED IN ELIMINATION FROM ORDERING VECTORS.
  220   KP = IP(JP,2)
        KL = IW(JP,2) + KP - 1
        DO 260 L = 1,2
          DO 250 K = KP,KL
            I = IND(K,L)
            IL = IW(I,L+4)
            IN = IW(I,L+6)
            IF (IL.EQ.0) GO TO 230
            IW(IL,L+6) = IN
            GO TO 240

  230       NZ = IW(I,L)
            IW(NZ,L+2) = IN
  240       IF (IN.GT.0) IW(IN,L+4) = IL
  250     CONTINUE
          KP = IP(IPP,1)
          KL = KP + IW(IPP,1) - 1
  260   CONTINUE
C STORE PIVOT
        IW(IPP,5) = -IPV
        IW(JP,6) = -IPV
C ELIMINATE PIVOTAL ROW FROM COLUMN FILE AND FIND PIVOT IN ROW FILE.
        DO 290 K = KP,KL
          J = IND(K,2)
          KPC = IP(J,2)
          IW(J,2) = IW(J,2) - 1
          KLC = KPC + IW(J,2)
          DO 270 KC = KPC,KLC
            IF (IPP.EQ.IND(KC,1)) GO TO 280
  270     CONTINUE
  280     IND(KC,1) = IND(KLC,1)
          IND(KLC,1) = 0
          IF (J.EQ.JP) KR = K
  290   CONTINUE
C BRING PIVOT TO FRONT OF PIVOTAL ROW.
        AU = A(KR)
        A(KR) = A(KP)
        A(KP) = AU
        IND(KR,2) = IND(KP,2)
        IND(KP,2) = JP
C
C PERFORM ELIMINATION ITSELF, LOOPING ON NON-ZEROS IN PIVOT COLUMN.
        NZC = IW(JP,2)
        IF (NZC.EQ.0) GO TO 520
        DO 510 NC = 1,NZC
          KC = IP(JP,2) + NC - 1
          IR = IND(KC,1)
C SEARCH NON-PIVOT ROW FOR ELEMENT TO BE ELIMINATED.
          KR = IP(IR,1)
          KRL = KR + IW(IR,1) - 1
          DO 300 KNP = KR,KRL
            IF (JP.EQ.IND(KNP,2)) GO TO 310
  300     CONTINUE
C BRING ELEMENT TO BE ELIMINATED TO FRONT OF ITS ROW.
  310     AM = A(KNP)
          A(KNP) = A(KR)
          A(KR) = AM
          IND(KNP,2) = IND(KR,2)
          IND(KR,2) = JP
          AM = -A(KR)/A(KP)
C COMPRESS ROW FILE UNLESS IT IS CERTAIN THAT THERE IS ROOM FOR NEW ROW.
          IF (LROW+IW(IR,1)+IW(IPP,1)+LENL.LE.IA) GO TO 320
          IF (NCP.GE.MCP .OR. LENU+IW(IR,1)+IW(IPP,1)+LENL.GT.
     +        IA) GO TO 670
          CALL LA15E(A,IND(1,2),IP,N,IW,IA,.TRUE.,NCP,LROW,LCOL)
          KP = IP(IPP,1)
          KR = IP(IR,1)
  320     KRL = KR + IW(IR,1) - 1
          KQ = KP + 1
          KPL = KP + IW(IPP,1) - 1
C PLACE PIVOT ROW (EXCLUDING PIVOT ITSELF) IN W.
          IF (KQ.GT.KPL) GO TO 340
          DO 330 K = KQ,KPL
            J = IND(K,2)
            W(J) = A(K)
  330     CONTINUE
  340     IP(IR,1) = LROW + 1
C
C TRANSFER MODIFIED ELEMENTS.
          IND(KR,2) = 0
          KR = KR + 1
          IF (KR.GT.KRL) GO TO 400
          DO 390 KS = KR,KRL
            J = IND(KS,2)
            AU = A(KS) + AM*W(J)
            IND(KS,2) = 0
C IF ELEMENT IS VERY SMALL REMOVE IT FROM U.
            IF (ABS(AU).LE.SMALL) GO TO 350
            G = MAX(G,ABS(AU))
            LROW = LROW + 1
            A(LROW) = AU
            IND(LROW,2) = J
            GO TO 380

  350       LENU = LENU - 1
C REMOVE ELEMENT FROM COL FILE.
            K = IP(J,2)
            KL = K + IW(J,2) - 1
            IW(J,2) = KL - K
            DO 360 KK = K,KL
              IF (IND(KK,1).EQ.IR) GO TO 370
  360       CONTINUE
  370       IND(KK,1) = IND(KL,1)
            IND(KL,1) = 0
  380       W(J) = 0.0
  390     CONTINUE
C
C SCAN PIVOT ROW FOR FILLS.
  400     IF (KQ.GT.KPL) GO TO 490
          DO 480 KS = KQ,KPL
            J = IND(KS,2)
            AU = AM*W(J)
            IF (ABS(AU).LE.SMALL) GO TO 470
            LROW = LROW + 1
            A(LROW) = AU
            IND(LROW,2) = J
            LENU = LENU + 1
C
C CREATE FILL IN COLUMN FILE.
            NZ = IW(J,2)
            K = IP(J,2)
            KL = K + NZ - 1
C IF POSSIBLE PLACE NEW ELEMENT AT END OF PRESENT ENTRY.
            IF (KL.NE.LCOL) GO TO 410
            IF (LCOL+LENL.GE.IA) GO TO 430
            LCOL = LCOL + 1
            GO TO 420

  410       IF (IND(KL+1,1).NE.0) GO TO 430
  420       IND(KL+1,1) = IR
            GO TO 460
C NEW ENTRY HAS TO BE CREATED.
  430       IF (LCOL+LENL+NZ+1.LT.IA) GO TO 440
C COMPRESS COLUMN FILE IF THERE IS NOT ROOM FOR NEW ENTRY.
            IF (NCP.GE.MCP .OR. LENU+LENL+NZ+1.GE.IA) GO TO 670
            CALL LA15E(A,IND,IP(1,2),N,IW(1,2),IA,.FALSE.,NCP,
     +                 LROW,LCOL)
            K = IP(J,2)
            KL = K + NZ - 1
C TRANSFER OLD ENTRY INTO NEW.
  440       IP(J,2) = LCOL + 1
            DO 450 KK = K,KL
              LCOL = LCOL + 1
              IND(LCOL,1) = IND(KK,1)
              IND(KK,1) = 0
  450       CONTINUE
C ADD NEW ELEMENT.
            LCOL = LCOL + 1
            IND(LCOL,1) = IR
  460       G = MAX(G,ABS(AU))
            IW(J,2) = NZ + 1
  470       W(J) = 0.0
  480     CONTINUE
  490     IW(IR,1) = LROW + 1 - IP(IR,1)
C
C STORE MULTIPLIER
          IF (LENL+LCOL+1.LE.IA) GO TO 500
C COMPRESS COL FILE IF NECESSARY.
          IF (NCP.GE.MCP) GO TO 670
          CALL LA15E(A,IND,IP(1,2),N,IW(1,2),IA,.FALSE.,NCP,
     +               LROW,LCOL)
  500     K = IA - LENL
          LENL = LENL + 1
          A(K) = AM
          IND(K,1) = IPP
          IND(K,2) = IR
          LENU = LENU - 1
  510   CONTINUE
C
C INSERT ROWS AND COLUMNS INVOLVED IN ELIMINATION IN LINKED LISTS
C     OF EQUAL NUMBERS OF NON-ZEROS.
  520   K1 = IP(JP,2)
        K2 = IW(JP,2) + K1 - 1
        IW(JP,2) = 0
        DO 550 L = 1,2
          IF (K2.LT.K1) GO TO 540
          DO 530 K = K1,K2
            IR = IND(K,L)
            IF (L.EQ.1) IND(K,L) = 0
            NZ = IW(IR,L)
            IF (NZ.GT.0) THEN
              IN = IW(NZ,L+2)
              IW(IR,L+6) = IN
              IW(NZ,L+2) = IR
              IF (IN.NE.0) IW(IN,L+4) = IR
            END IF
            IW(IR,L+4) = 0
  530     CONTINUE
  540     K1 = IP(IPP,1) + 1
          K2 = IW(IPP,1) + K1 - 2
  550   CONTINUE
  560 CONTINUE
C
C RESET COLUMN FILE TO REFER TO U AND STORE ROW/COL NUMBERS IN
C     PIVOTAL ORDER IN IW(.,3),IW(.,4)
      DO 570 I = 1,N
        J = -IW(I,5)
        IW(J,3) = I
        J = -IW(I,6)
        IW(J,4) = I
        IW(I,2) = 0
  570 CONTINUE
      DO 590 I = 1,N
        KP = IP(I,1)
        KL = IW(I,1) + KP - 1
        DO 580 K = KP,KL
          J = IND(K,2)
          IW(J,2) = IW(J,2) + 1
  580   CONTINUE
  590 CONTINUE
      K = 1
      DO 600 I = 1,N
        K = K + IW(I,2)
        IP(I,2) = K
  600 CONTINUE
      LCOL = K - 1
      DO 620 II = 1,N
        I = IW(II,3)
        KP = IP(I,1)
        KL = IW(I,1) + KP - 1
        DO 610 K = KP,KL
          J = IND(K,2)
          KN = IP(J,2) - 1
          IP(J,2) = KN
          IND(KN,1) = I
  610   CONTINUE
  620 CONTINUE
      GO TO 710
C
C     THE FOLLOWING INSTRUCTIONS IMPLEMENT THE FAILURE EXITS.
  630 IF (LP.GT.0) WRITE (LP,FMT=9000) IR,J

 9000 FORMAT (/,/,' ERROR RETURN FROM LA15A BECAUSE',/,' THERE IS MORE',
     +       ' THAN ONE ENTRY IN ROW',I5,' AND COLUMN',I5)

      G = -4.
      GO TO 710

  640 IF (LP.GT.0) WRITE (LP,FMT=9010)

 9010 FORMAT (/,/,' ERROR RETURN FROM LA15A BECAUSE N IS NOT POSITIVE')

      G = -1.
      GO TO 710

  650 IF (LP.GT.0) WRITE (LP,FMT=9020) K,I,J

 9020 FORMAT (/,/,' ERROR RETURN FROM LA15A BECAUSE',/,' ELEMENT',I7,
     +       ' IS IN ROW',I5,' AND COLUMN',I5)

      G = -3.
      GO TO 710

  670 IF (LP.GT.0) WRITE (LP,FMT=9040)

 9040 FORMAT (/,/,' ERROR RETURN FROM LA15A BECAUSE IA IS TOO SMALL')

      G = -7.
      GO TO 710

  680 IPV = IPV - 1
      IF (LP.GT.0) WRITE (LP,FMT=9050) IPV
 9050 FORMAT (/,/,' ERROR RETURN FROM LA15A BECAUSE THE MATRIX IS ',
     +   'SINGULAR WITH RANK ',I7)
      G = -5.
      GO TO 710

  700 IF (LP.GT.0) WRITE (LP,FMT=9060)

 9060 FORMAT (/,/,' ERROR RETURN FROM LA15A BECAUSE NZ IS NEGATIVE')

      G = -8.
C
C  Save kept data and return to caller
C
  710 CONTINUE
      KEEP(1)  = LENL
      KEEP(2)  = LENU
      KEEP(3)  = NCP
      KEEP(4)  = LROW
      KEEP(5)  = LCOL
      RETURN
      END
      SUBROUTINE LA15B(A,IND,IA,N,IP,IW,W,G,B,TRANS,ICNTL,KEEP)
C IP(I,1),IP(I,2) POINT TO START OF ROW/COLUMN I OF U.
C IW(I,1),IW(I,2) ARE LENGTHS OF ROW/COL I OF U.
C IW(.,3),IW(.,4) HOLD ROW/COL NUMBERS IN PIVOTAL ORDER.
C     .. Scalar Arguments ..
      REAL G
      INTEGER IA,N
      LOGICAL TRANS
C     ..
C     .. Array Arguments ..
      REAL A(IA),B(N),W(N)
      INTEGER IND(IA,2),IP(N,2),IW(N,4)
      INTEGER ICNTL(3)
      INTEGER KEEP(7)
C     ..
C     .. Local Scalars ..
      REAL AM
      INTEGER I,II,J,K,K2,KK,KL,KLL,KP,KPC,L1,N1,NZ,LP,LENL
C     ..
C     .. Intrinsic Functions ..
      INTRINSIC ABS
C     .. Executable Statements ..
C
C  Restore kept data (not altered by LA15B)
C
      LP    = ICNTL(1)
      LENL  = KEEP(1)
C
      IF (G.LT.0.0) GO TO 130
      KLL = IA - LENL + 1
      IF (TRANS) GO TO 80
C
C     MULTIPLY VECTOR BY INVERSE OF L
      IF (LENL.LE.0) GO TO 20
      L1 = IA + 1
      DO 10 KK = 1,LENL
        K = L1 - KK
        I = IND(K,1)
        IF (B(I).EQ.0.0) GO TO 10
        J = IND(K,2)
        B(J) = B(J) + A(K)*B(I)
   10 CONTINUE
   20 DO 30 I = 1,N
        W(I) = B(I)
        B(I) = 0.0
   30 CONTINUE
C
C     MULTIPLY VECTOR BY INVERSE OF U
      N1 = N + 1
      DO 70 II = 1,N
        I = N1 - II
        I = IW(I,3)
        AM = W(I)
        KP = IP(I,1)
        IF (KP.GT.0) GO TO 50
        KP = -KP
        IP(I,1) = KP
        NZ = IW(I,1)
        KL = KP - 1 + NZ
        K2 = KP + 1
        DO 40 K = K2,KL
          J = IND(K,2)
          AM = AM - A(K)*B(J)
   40   CONTINUE
   50   IF (AM.EQ.0.0) GO TO 70
        J = IND(KP,2)
        B(J) = AM/A(KP)
        KPC = IP(J,2)
        KL = IW(J,2) + KPC - 1
        IF (KL.EQ.KPC) GO TO 70
        K2 = KPC + 1
        DO 60 K = K2,KL
          I = IND(K,1)
          IP(I,1) = -ABS(IP(I,1))
   60   CONTINUE
   70 CONTINUE
      GO TO 140
C
C     MULTIPLY VECTOR BY INVERSE OF TRANSPOSE OF U
   80 DO 90 I = 1,N
        W(I) = B(I)
        B(I) = 0.0
   90 CONTINUE
      DO 110 II = 1,N
        I = IW(II,4)
        AM = W(I)
        IF (AM.EQ.0.0) GO TO 110
        J = IW(II,3)
        KP = IP(J,1)
        AM = AM/A(KP)
        B(J) = AM
        KL = IW(J,1) + KP - 1
        IF (KP.EQ.KL) GO TO 110
        K2 = KP + 1
        DO 100 K = K2,KL
          I = IND(K,2)
          W(I) = W(I) - AM*A(K)
  100   CONTINUE
  110 CONTINUE
C
C     MULTIPLY VECTOR BY INVERSE OF TRANSPOSE OF L
      IF (KLL.GT.IA) RETURN
      DO 120 K = KLL,IA
        J = IND(K,2)
        IF (B(J).EQ.0.0) GO TO 120
        I = IND(K,1)
        B(I) = B(I) + A(K)*B(J)
  120 CONTINUE
      GO TO 140
C
  130 IF (LP.GT.0) WRITE (LP,FMT=9000)

 9000 FORMAT (/,/,' ERROR RETURN FROM LA15B BECAUSE EARLIER ENTRY',' G',
     +       'AVE ERROR RETURN')
  140 RETURN
      END
      SUBROUTINE LA15C(A,IND,IA,N,IP,IW,W,G,U,MM,ICNTL,CNTL,KEEP)
C     .. Scalar Arguments ..
      REAL G,U
      INTEGER IA,MM,N
C     ..
C     .. Array Arguments ..
      REAL A(IA),W(N)
      INTEGER IND(IA,2),IP(N,2),IW(N,4)
      REAL CNTL(3)
      INTEGER ICNTL(3)
      INTEGER KEEP(7)
C     ..
C     .. Local Scalars ..
      REAL AM,AU
      INTEGER I,II,IJ,IM,IN,INS,IPP,IR,IS,J,JM,JNS,JP,K,KJ,KK,KL,KM,KNP,
     +        KP,KPL,KQ,KR,KRL,KS,L,LAST,LAST1,LAST2,M,M1,MCP,NZ
      INTEGER LP,LENL,LENU,NCP,LROW,LCOL
      REAL SMALL
C     ..
C     .. External Subroutines ..
      EXTERNAL LA15E
C     ..
C     .. Intrinsic Functions ..
      INTRINSIC ABS,MAX
C     .. Executable Statements ..
C
C  Restore kept data
C
      LP    = ICNTL(1)
      SMALL = CNTL(1)
      LENL  = KEEP(1)
      LENU  = KEEP(2)
      NCP   = KEEP(3)
      LROW  = KEEP(4)
      LCOL  = KEEP(5)
C
      IF (G.LT.0.0) GO TO 620
      IF (MM.LT.1 .OR. MM.GT.N) GO TO 630
      JM = MM
C MCP LIMITS THE VALUE OF NCP PERMITTED BEFORE AN ERROR RETURN RESULTS.
      MCP = NCP + 20
C REMOVE OLD COLUMN
      LENU = LENU - IW(JM,2)
      KP = IP(JM,2)
      IM = IND(KP,1)
      KL = KP + IW(JM,2) - 1
      IW(JM,2) = 0
      DO 30 K = KP,KL
        I = IND(K,1)
        IND(K,1) = 0
        KR = IP(I,1)
        NZ = IW(I,1) - 1
        IW(I,1) = NZ
        KRL = KR + NZ
        DO 10 KM = KR,KRL
          IF (IND(KM,2).EQ.JM) GO TO 20
   10   CONTINUE
   20   A(KM) = A(KRL)
        IND(KM,2) = IND(KRL,2)
        IND(KRL,2) = 0
   30 CONTINUE
C
C INSERT NEW COLUMN
C JDH 09/04/13: Added initialization of LAST. I /think/ 0 is the correct
C value, but can't be certain. It seems to work.
      LAST = 0
      DO 110 II = 1,N
        I = IW(II,3)
        IF (I.EQ.IM) M = II
        IF (ABS(W(I)).LE.SMALL) GO TO 100
        LENU = LENU + 1
        LAST = II
        IF (LCOL+LENL.LT.IA) GO TO 40
C COMPRESS COLUMN FILE IF NECESSARY.
        IF (NCP.GE.MCP .OR. LENL+LENU.GE.IA) GO TO 610
        CALL LA15E(A,IND,IP(1,2),N,IW(1,2),IA,.FALSE.,NCP,LROW,
     +             LCOL)
   40   LCOL = LCOL + 1
        NZ = IW(JM,2)
        IF (NZ.EQ.0) IP(JM,2) = LCOL
        IW(JM,2) = NZ + 1
        IND(LCOL,1) = I
        NZ = IW(I,1)
        KPL = IP(I,1) + NZ
        IF (KPL.GT.LROW) GO TO 50
        IF (IND(KPL,2).EQ.0) GO TO 90
C NEW ENTRY HAS TO BE CREATED.
   50   IF (LENL+LROW+NZ.LT.IA) GO TO 60
        IF (NCP.GE.MCP .OR. LENL+LENU+NZ.GE.IA) GO TO 610
C COMPRESS ROW FILE IF NECESSARY.
        CALL LA15E(A,IND(1,2),IP,N,IW,IA,.TRUE.,NCP,LROW,LCOL)
   60   KP = IP(I,1)
        IP(I,1) = LROW + 1
        IF (NZ.EQ.0) GO TO 80
        KPL = KP + NZ - 1
        DO 70 K = KP,KPL
          LROW = LROW + 1
          A(LROW) = A(K)
          IND(LROW,2) = IND(K,2)
          IND(K,2) = 0
   70   CONTINUE
   80   LROW = LROW + 1
        KPL = LROW
C PLACE NEW ELEMENT AT END OF ROW.
   90   IW(I,1) = NZ + 1
        A(KPL) = W(I)
        IND(KPL,2) = JM
  100   W(I) = 0.0
  110 CONTINUE
      IF (IW(IM,1).EQ.0 .OR. IW(JM,2).EQ.0 .OR. M.GT.LAST) GO TO 600
C
C FIND COLUMN SINGLETONS, OTHER THAN THE SPIKE. NON-SINGLETONS ARE
C     MARKED WITH W(J)=1. ONLY IW(.,3) IS REVISED AND IW(.,4) IS USED
C     FOR WORKSPACE.
      INS = M
      M1 = M
      W(JM) = 1.
      DO 140 II = M,LAST
        I = IW(II,3)
        J = IW(II,4)
        IF (W(J).EQ.0.0) GO TO 130
        KP = IP(I,1)
        KL = KP + IW(I,1) - 1
        DO 120 K = KP,KL
          J = IND(K,2)
          W(J) = 1.
  120   CONTINUE
        IW(INS,4) = I
        INS = INS + 1
        GO TO 140
C PLACE SINGLETONS IN NEW POSITION.
  130   IW(M1,3) = I
        M1 = M1 + 1
  140 CONTINUE
C PLACE NON-SINGLETONS IN NEW POSITION.
      IJ = M + 1
      IF (M1.GE.LAST) GO TO 160
      LAST1 = LAST - 1
      DO 150 II = M1,LAST1
        IW(II,3) = IW(IJ,4)
        IJ = IJ + 1
  150 CONTINUE
C PLACE SPIKE AT END.
  160 IW(LAST,3) = IM
C
C FIND ROW SINGLETONS, APART FROM SPIKE ROW. NON-SINGLETONS ARE MARKED
C     WITH W(I)=2. AGAIN ONLY IW(.,3) IS REVISED AND IW(.,4) IS USED
C     FOR WORKSPACE.
      LAST1 = LAST
      JNS = LAST
      W(IM) = 2.
      J = JM
      DO 190 IJ = M1,LAST
        II = LAST + M1 - IJ
        I = IW(II,3)
        IF (W(I).NE.2.) GO TO 180
        K = IP(I,1)
        IF (II.NE.LAST) J = IND(K,2)
        KP = IP(J,2)
        KL = KP + IW(J,2) - 1
        IW(JNS,4) = I
        JNS = JNS - 1
        DO 170 K = KP,KL
          I = IND(K,1)
          W(I) = 2.
  170   CONTINUE
        GO TO 190

  180   IW(LAST1,3) = I
        LAST1 = LAST1 - 1
  190 CONTINUE
      DO 200 II = M1,LAST1
        JNS = JNS + 1
        I = IW(JNS,4)
        W(I) = 3.
        IW(II,3) = I
  200 CONTINUE
C
C DEAL WITH SINGLETON SPIKE COLUMN. NOTE THAT BUMP ROWS ARE MARKED BY
C    W(I)=3.
      DO 240 II = M1,LAST1
        KP = IP(JM,2)
        KL = KP + IW(JM,2) - 1
        IS = 0
        DO 210 K = KP,KL
          L = IND(K,1)
          IF (W(L).NE.3.) GO TO 210
          IF (IS.NE.0) GO TO 250
          I = L
          KNP = K
          IS = 1
  210   CONTINUE
        IF (IS.EQ.0) GO TO 600
C MAKE A(I,JM) A PIVOT.
        IND(KNP,1) = IND(KP,1)
        IND(KP,1) = I
        KP = IP(I,1)
        DO 220 K = KP,IA
          IF (IND(K,2).EQ.JM) GO TO 230
  220   CONTINUE
  230   AM = A(KP)
        A(KP) = A(K)
        A(K) = AM
        IND(K,2) = IND(KP,2)
        IND(KP,2) = JM
        JM = IND(K,2)
        IW(II,4) = I
        W(I) = 2.
  240 CONTINUE
      II = LAST1
      GO TO 270

  250 IN = M1
      DO 260 IJ = II,LAST1
        IW(IJ,4) = IW(IN,3)
        IN = IN + 1
  260 CONTINUE
  270 LAST2 = LAST1 - 1
      IF (M1.EQ.LAST1) GO TO 580
      DO 280 I = M1,LAST2
        IW(I,3) = IW(I,4)
  280 CONTINUE
      M1 = II
      IF (M1.EQ.LAST1) GO TO 580
C
C CLEAR W
      DO 290 I = 1,N
        W(I) = 0.0
  290 CONTINUE
C
C PERFORM ELIMINATION
      IR = IW(LAST1,3)
      DO 570 II = M1,LAST1
        IPP = IW(II,3)
        KP = IP(IPP,1)
        KR = IP(IR,1)
        JP = IND(KP,2)
        IF (II.EQ.LAST1) JP = JM
C SEARCH NON-PIVOT ROW FOR ELEMENT TO BE ELIMINATED.
C  AND BRING IT TO FRONT OF ITS ROW
        KRL = KR + IW(IR,1) - 1
        DO 300 KNP = KR,KRL
          IF (JP.EQ.IND(KNP,2)) GO TO 310
  300   CONTINUE
        IF(II-LAST1.NE.0) GO TO 570
        IF(II-LAST1.EQ.0) GO TO 600
C BRING ELEMENT TO BE ELIMINATED TO FRONT OF ITS ROW.
  310   AM = A(KNP)
        A(KNP) = A(KR)
        A(KR) = AM
        IND(KNP,2) = IND(KR,2)
        IND(KR,2) = JP
        IF (II.EQ.LAST1) GO TO 320
        IF (ABS(A(KP)).LT.U*ABS(AM)) GO TO 320
        IF (ABS(AM).LT.U*ABS(A(KP))) GO TO 350
        IF (IW(IPP,1).LE.IW(IR,1)) GO TO 350
C PERFORM INTERCHANGE
  320   IW(LAST1,3) = IPP
        IW(II,3) = IR
        IR = IPP
        IPP = IW(II,3)
        K = KR
        KR = KP
        KP = K
        KJ = IP(JP,2)
        DO 330 K = KJ,IA
          IF (IND(K,1).EQ.IPP) GO TO 340
  330   CONTINUE
  340   IND(K,1) = IND(KJ,1)
        IND(KJ,1) = IPP
  350   IF (A(KP).EQ.0.0) GO TO 600
        IF (II.EQ.LAST1) GO TO 570
        AM = -A(KR)/A(KP)
C COMPRESS ROW FILE UNLESS IT IS CERTAIN THAT THERE IS ROOM FOR NEW ROW.
        IF (LROW+IW(IR,1)+IW(IPP,1)+LENL.LE.IA) GO TO 360
        IF (NCP.GE.MCP .OR. LENU+IW(IR,1)+IW(IPP,1)+LENL.GT.
     +      IA) GO TO 610
        CALL LA15E(A,IND(1,2),IP,N,IW,IA,.TRUE.,NCP,LROW,LCOL)
        KP = IP(IPP,1)
        KR = IP(IR,1)
  360   KRL = KR + IW(IR,1) - 1
        KQ = KP + 1
        KPL = KP + IW(IPP,1) - 1
C PLACE PIVOT ROW (EXCLUDING PIVOT ITSELF) IN W.
        IF (KQ.GT.KPL) GO TO 380
        DO 370 K = KQ,KPL
          J = IND(K,2)
          W(J) = A(K)
  370   CONTINUE
  380   IP(IR,1) = LROW + 1
C
C TRANSFER MODIFIED ELEMENTS.
        IND(KR,2) = 0
        KR = KR + 1
        IF (KR.GT.KRL) GO TO 440
        DO 430 KS = KR,KRL
          J = IND(KS,2)
          AU = A(KS) + AM*W(J)
          IND(KS,2) = 0
C IF ELEMENT IS VERY SMALL REMOVE IT FROM U.
          IF (ABS(AU).LE.SMALL) GO TO 390
          G = MAX(G,ABS(AU))
          LROW = LROW + 1
          A(LROW) = AU
          IND(LROW,2) = J
          GO TO 420

  390     LENU = LENU - 1
C REMOVE ELEMENT FROM COL FILE.
          K = IP(J,2)
          KL = K + IW(J,2) - 1
          IW(J,2) = KL - K
          DO 400 KK = K,KL
            IF (IND(KK,1).EQ.IR) GO TO 410
  400     CONTINUE
  410     IND(KK,1) = IND(KL,1)
          IND(KL,1) = 0
  420     W(J) = 0.0
  430   CONTINUE
C
C SCAN PIVOT ROW FOR FILLS.
  440   IF (KQ.GT.KPL) GO TO 530
        DO 520 KS = KQ,KPL
          J = IND(KS,2)
          AU = AM*W(J)
          IF (ABS(AU).LE.SMALL) GO TO 510
          LROW = LROW + 1
          A(LROW) = AU
          IND(LROW,2) = J
          LENU = LENU + 1
C
C CREATE FILL IN COLUMN FILE.
          NZ = IW(J,2)
          K = IP(J,2)
          KL = K + NZ - 1
C IF POSSIBLE PLACE NEW ELEMENT AT END OF PRESENT ENTRY.
          IF (KL.NE.LCOL) GO TO 450
          IF (LCOL+LENL.GE.IA) GO TO 470
          LCOL = LCOL + 1
          GO TO 460

  450     IF (IND(KL+1,1).NE.0) GO TO 470
  460     IND(KL+1,1) = IR
          GO TO 500
C NEW ENTRY HAS TO BE CREATED.
  470     IF (LCOL+LENL+NZ+1.LT.IA) GO TO 480
C COMPRESS COLUMN FILE IF THERE IS NOT ROOM FOR NEW ENTRY.
          IF (NCP.GE.MCP .OR. LENU+LENL+NZ+1.GE.IA) GO TO 610
          CALL LA15E(A,IND,IP(1,2),N,IW(1,2),IA,.FALSE.,NCP,
     +               LROW,LCOL)
          K = IP(J,2)
          KL = K + NZ - 1
C TRANSFER OLD ENTRY INTO NEW.
  480     IP(J,2) = LCOL + 1
          DO 490 KK = K,KL
            LCOL = LCOL + 1
            IND(LCOL,1) = IND(KK,1)
            IND(KK,1) = 0
  490     CONTINUE
C ADD NEW ELEMENT.
          LCOL = LCOL + 1
          IND(LCOL,1) = IR
  500     G = MAX(G,ABS(AU))
          IW(J,2) = NZ + 1
  510     W(J) = 0.0
  520   CONTINUE
  530   IW(IR,1) = LROW + 1 - IP(IR,1)
C
C STORE MULTIPLIER
        IF (LENL+LCOL+1.LE.IA) GO TO 540
C COMPRESS COL FILE IF NECESSARY.
        IF (NCP.GE.MCP) GO TO 610
        CALL LA15E(A,IND,IP(1,2),N,IW(1,2),IA,.FALSE.,NCP,LROW,
     +             LCOL)
  540   K = IA - LENL
        LENL = LENL + 1
        A(K) = AM
        IND(K,1) = IPP
        IND(K,2) = IR
C CREATE BLANK IN PIVOTAL COLUMN.
        KP = IP(JP,2)
        NZ = IW(JP,2) - 1
        KL = KP + NZ
        DO 550 K = KP,KL
          IF (IND(K,1).EQ.IR) GO TO 560
  550   CONTINUE
  560   IND(K,1) = IND(KL,1)
        IW(JP,2) = NZ
        IND(KL,1) = 0
        LENU = LENU - 1
  570 CONTINUE
C
C CONSTRUCT COLUMN PERMUTATION AND STORE IT IN IW(.,4)
  580 DO 590 II = M,LAST
        I = IW(II,3)
        K = IP(I,1)
        J = IND(K,2)
        IW(II,4) = J
  590 CONTINUE
      GO TO 640
C
C     THE FOLLOWING INSTRUCTIONS IMPLEMENT THE FAILURE EXITS.
  600 IF (LP.NE.0) WRITE (LP,FMT=9000) MM

 9000 FORMAT (/,/,' ERROR RETURN FROM LA15C BECAUSE',/,' SINGULAR MATR',
     +       'IX CREATED BY REPLACEMENT OF COL',I5)

      G = -6.
      GO TO 640

  610 IF (LP.NE.0) WRITE (LP,FMT=9010)

 9010 FORMAT (/,/,' ERROR RETURN FROM LA15C BECAUSE IA IS TOO SMALL')

      G = -7.
      GO TO 640

  620 IF (LP.NE.0) WRITE (LP,FMT=9020)

 9020 FORMAT (/,/,' ERROR RETURN FROM LA15C BECAUSE EARLIER ENTRY',' G',
     +       'AVE ERROR RETURN')

      GO TO 640

  630 IF (LP.NE.0) WRITE (LP,FMT=9030) MM

 9030 FORMAT (/,/,' ERROR RETURN FROM LA15C BECAUSE M HAS THE VALUE',I8)

      G = -9.
C
C  Save kept data and return to caller
C
  640 CONTINUE
      KEEP(1)  = LENL
      KEEP(2)  = LENU
      KEEP(3)  = NCP
      KEEP(4)  = LROW
      KEEP(5)  = LCOL
      RETURN
      END
      SUBROUTINE LA15E(A,IRN,IP,N,IW,IA,REALS,NCP,LROW,LCOL)
C     .. Scalar Arguments ..
      INTEGER IA,N,NCP,LROW,LCOL
      LOGICAL REALS
C     ..
C     .. Array Arguments ..
      REAL A(IA)
      INTEGER IP(N),IRN(IA),IW(N)
C     ..
C     .. Local Scalars ..
      INTEGER IPI,J,K,KL,KN,NZ
C     ..
C     .. Executable Statements ..
      NCP = NCP + 1
C     COMPRESS FILE OF POSITIVE INTEGERS. ENTRY J STARTS AT IRN(IP(J))
C  AND CONTAINS IW(J) INTEGERS,J=1,N. OTHER COMPONENTS OF IRN ARE ZERO.
C  LENGTH OF COMPRESSED FILE PLACED IN LROW IF REALS IS .TRUE. OR LCOL
C  OTHERWISE.
C  IF REALS IS .TRUE. ARRAY A CONTAINS A REAL FILE ASSOCIATED WITH IRN
C  AND THIS IS COMPRESSED TOO.
C  A,IRN,IP,IW,IA ARE INPUT/OUTPUT VARIABLES.
C  N,REALS ARE INPUT/UNCHANGED VARIABLES.
C
      DO 10 J = 1,N
C STORE THE LAST ELEMENT OF ENTRY J IN IW(J) THEN OVERWRITE IT BY -J.
        NZ = IW(J)
        IF (NZ.LE.0) GO TO 10
        K = IP(J) + NZ - 1
        IW(J) = IRN(K)
        IRN(K) = -J
   10 CONTINUE
C KN IS THE POSITION OF NEXT ENTRY IN COMPRESSED FILE.
      KN = 0
      IPI = 0
      KL = LCOL
      IF (REALS) KL = LROW
C LOOP THROUGH THE OLD FILE SKIPPING ZERO (DUMMY) ELEMENTS AND
C     MOVING GENUINE ELEMENTS FORWARD. THE ENTRY NUMBER BECOMES
C     KNOWN ONLY WHEN ITS END IS DETECTED BY THE PRESENCE OF A NEGATIVE
C     INTEGER.
      DO 30 K = 1,KL
        IF (IRN(K).EQ.0) GO TO 30
        KN = KN + 1
        IF (REALS) A(KN) = A(K)
        IF (IRN(K).GE.0) GO TO 20
C END OF ENTRY. RESTORE IRN(K), SET POINTER TO START OF ENTRY AND
C     STORE CURRENT KN IN IPI READY FOR USE WHEN NEXT LAST ENTRY
C     IS DETECTED.
        J = -IRN(K)
        IRN(K) = IW(J)
        IP(J) = IPI + 1
        IW(J) = KN - IPI
        IPI = KN
   20   IRN(KN) = IRN(K)
   30 CONTINUE
      IF (REALS) LROW = KN
      IF (.NOT.REALS) LCOL = KN
      RETURN
      END
* COPYRIGHT (c) 1993 AEA Technology and
* Council for the Central Laboratory of the Research Councils
C Original date March 1993
C 12th July 2004 Version 1.0.0. Version numbering added.

      SUBROUTINE MC29A(M,N,NE,A,IRN,ICN,R,C,W,LP,IFAIL)
      INTEGER M,N,NE
      REAL A(NE)
      INTEGER IRN(NE),ICN(NE)
      REAL R(M),C(N),W(M*2+N*3)
      INTEGER LP,IFAIL
C M is an integer variable that must be set to the number of rows.
C      It is not altered by the subroutine.
C N is an integer variable that must be set to the number of columns.
C      It is not altered by the subroutine.
C NE is an integer variable that must be set to the number of entries.
C      It is not altered by the subroutine.
C A is an array that holds the values of the entries.
C IRN  is an integer array that must be set to the row indices of the
C      entries. It is not altered by the subroutine.
C ICN  is an integer array that must be set to the column indices of the
C      entries. It is not altered by the subroutine.
C R is an array that need not be set on entry. On return, it holds the
C      logarithms of the row scaling factors.
C C is an array that need not be set on entry. On return, it holds the
C      logarithms of the column scaling factors.
C W is a workarray.
C      W(1:M)  holds row non-zero counts (diagonal matrix M).
C      W(M+1:M+N) holds column non-zero counts (diagonal matrix N).
C      W(M+N+J) holds the logarithm of the column I scaling
C         factor during the iteration, J=1,2,...,N.
C      W(M+N*2+J) holds the 2-iteration change in the logarithm
C         of the column J scaling factor, J=1,2,...,N.
C      W(M+N*3+I) is used to save the average logarithm of
C          the entries of row I, I=1,2,...,M.
C LP must be set to the unit number for messages.
C      It is not altered by the subroutine.
C IFAIL need not be set by the user. On return it has one of the
C     following values:
C     0 successful entry.
C     -1 M < 1 or N < 1.
C     -2 NE < 1.

      INTRINSIC LOG,ABS,MIN

C Constants
      INTEGER MAXIT
      PARAMETER (MAXIT=100)
      REAL ONE,SMIN,ZERO
      PARAMETER (ONE=1.0,SMIN=0.1,ZERO=0.0)
C MAXIT is the maximal permitted number of iterations.
C SMIN is used in a convergence test on (residual norm)**2

C Local variables
      INTEGER I,I1,I2,I3,I4,I5,ITER,J,K
      REAL E,E1,EM,Q,Q1,QM,S,S1,SM,U,V

C Check M, N and NE.
      IFAIL = 0
      IF (M.LT.1 .OR. N.LT.1) THEN
         IFAIL = -1
         GO TO 220
      ELSE IF (NE.LE.0) THEN
         IFAIL = -2
         GO TO 220
      END IF

C     Partition W
      I1 = 0
      I2 = M
      I3 = M + N
      I4 = M + N*2
      I5 = M + N*3

C     Initialise for accumulation of sums and products.
      DO 10 I = 1,M
         R(I) = ZERO
         W(I1+I) = ZERO
   10 CONTINUE
      DO 20 J = 1,N
         C(J) = ZERO
         W(I2+J) = ZERO
         W(I3+J) = ZERO
         W(I4+J) = ZERO
   20 CONTINUE

C     Count non-zeros in the rows, and compute rhs vectors.
      DO 30 K = 1,NE
         U = ABS(A(K))
         IF (U.EQ.ZERO) GO TO 30
         I = IRN(K)
         J = ICN(K)
         IF (MIN(I,J).LT.1 .OR. I.GT.M .OR. J.GT.N) GO TO 30
         U = LOG(U)
         W(I1+I) = W(I1+I) + ONE
         W(I2+J) = W(I2+J) + ONE
         R(I) = R(I) + U
         W(I3+J) = W(I3+J) + U
   30 CONTINUE
C
C     Divide rhs by diag matrices.
      DO 40 I = 1,M
         IF (W(I1+I).EQ.ZERO) W(I1+I) = ONE
         R(I) = R(I)/W(I1+I)
C     Save R(I) for use at end.
         W(I5+I) = R(I)
   40 CONTINUE
      DO 50 J = 1,N
         IF (W(I2+J).EQ.ZERO) W(I2+J) = ONE
         W(I3+J) = W(I3+J)/W(I2+J)
   50 CONTINUE
      SM = SMIN*NE

C     Sweep to compute initial residual vector
      DO 60 K = 1,NE
         IF (A(K).EQ.ZERO) GO TO 60
         I = IRN(K)
         J = ICN(K)
         IF (MIN(I,J).LT.1 .OR. I.GT.M .OR. J.GT.N) GO TO 60
         R(I) = R(I) - W(I3+J)/W(I1+I)
   60 CONTINUE
C
C     Initialise iteration
      E = ZERO
      Q = ONE
      S = ZERO
      DO 70 I = 1,M
         S = S + W(I1+I)*R(I)**2
   70 CONTINUE
      IF (S.LE.SM) GO TO 160

C     Iteration loop
      DO 150 ITER = 1,MAXIT
C    Sweep through matrix to update residual vector
         DO 80 K = 1,NE
            IF (A(K).EQ.ZERO) GO TO 80
            J = ICN(K)
            I = IRN(K)
            IF (MIN(I,J).LT.1 .OR. I.GT.M .OR. J.GT.N) GO TO 80
            C(J) = C(J) + R(I)
   80    CONTINUE
         S1 = S
         S = ZERO
         DO 90 J = 1,N
            V = -C(J)/Q
            C(J) = V/W(I2+J)
            S = S + V*C(J)
   90    CONTINUE
         E1 = E
         E = Q*S/S1
         Q = ONE - E
C      write(*,'(a,i3,a,f12.4)')' Iteration',ITER,' S =',S
         IF (S.LE.SM) E = ZERO
C     Update residual.
         DO 100 I = 1,M
            R(I) = R(I)*E*W(I1+I)
  100    CONTINUE
         IF (S.LE.SM) GO TO 180
         EM = E*E1
C    Sweep through matrix to update residual vector
         DO 110 K = 1,NE
            IF (A(K).EQ.ZERO) GO TO 110
            I = IRN(K)
            J = ICN(K)
            IF (MIN(I,J).LT.1 .OR. I.GT.M .OR. J.GT.N) GO TO 110
            R(I) = R(I) + C(J)
  110    CONTINUE
         S1 = S
         S = ZERO
         DO 120 I = 1,M
            V = -R(I)/Q
            R(I) = V/W(I1+I)
            S = S + V*R(I)
  120    CONTINUE
         E1 = E
         E = Q*S/S1
         Q1 = Q
         Q = ONE - E
C     Special fixup for last iteration.
         IF (S.LE.SM) Q = ONE
C     Update col. scaling powers
         QM = Q*Q1
         DO 130 J = 1,N
            W(I4+J) = (EM*W(I4+J)+C(J))/QM
            W(I3+J) = W(I3+J) + W(I4+J)
  130    CONTINUE
C      write(*,'(a,i3,a,f12.4)')' Iteration',ITER,' S =',S
         IF (S.LE.SM) GO TO 160
C     UPDATE RESIDUAL.
         DO 140 J = 1,N
            C(J) = C(J)*E*W(I2+J)
  140    CONTINUE
  150 CONTINUE
  160 DO 170 I = 1,M
         R(I) = R(I)*W(I1+I)
  170 CONTINUE
C
C     Sweep through matrix to prepare to get row scaling powers
  180 DO 190 K = 1,NE
         IF (A(K).EQ.ZERO) GO TO 190
         I = IRN(K)
         J = ICN(K)
         IF (MIN(I,J).LT.1 .OR. I.GT.M .OR. J.GT.N) GO TO 190
         R(I) = R(I) + W(I3+J)
  190 CONTINUE
C
C     Final conversion to output values.
      DO 200 I = 1,M
         R(I) = R(I)/W(I1+I) - W(I5+I)
  200 CONTINUE
      DO 210 J = 1,N
         C(J) = -W(I3+J)
  210 CONTINUE
      RETURN

C Error returns
  220 IF (LP.GT.0) WRITE (LP,'(/A/A,I3)')
     +    ' **** Error return from MC29A ****',' IFAIL =',IFAIL

      END
