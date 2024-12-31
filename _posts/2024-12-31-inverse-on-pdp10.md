---
layout: post
title: "inverse on pdp10"
---

example to help discussion around 7090/7094 'make more sense'. this is project inverse. also working on project simplex, in addition to matrix inversion. these two were 'cash cows' for ibm in the fifties and sixties.

got the code in docs/inverse going and it was a big help working on this as well as decwar itself. learning new things in unexpected places and ways! for example, didn't expect to ever fall in love with kermit's scripting capabilities. but yesterday that happened.

all was part of a push to get project inverse running on pdp10, with good workflows over from linux. this code is from an ibm. backus and ibm did fortran for exactly this type of thing, and 7090/7094 was 'a standard'. seems inevitable that this fortran iv code should 'live on' with linux, pdp10, 7090/7094. one reasonable thing to say here is simply, 'thirty-six bits'. and ask, was pdp6/10 an effort to challenge big blue 'directly' in this regard?

    .EXEC INVERSE
    FORTRAN: INVERS
    MAIN.   
    INVERT  
    DECOMP  
    SOLVE   
    SING    
    INVHIL  
    PRINT1  
    PRINT2  
    LINK:   Loading
    [LNKXCT INVERS execution]
    
          36.00     -630.00     3360.00    -7560.00     7560.00    -2772.00
        -630.00    14700.00   -88200.00   211680.00  -220500.00    83160.00
        3360.00   -88200.00   564480.00 -1411199.98  1512000.02  -582120.00
       -7560.00   211680.00 -1411199.98  3628800.03 -3969000.00  1552320.02
        7560.00  -220500.00  1512000.02 -3969000.00  4410000.00 -1746359.98
       -2772.00    83160.00  -582120.00  1552320.02 -1746359.98   698544.02
    
     0.98892106  0.49116241  0.32595560  0.24365902  0.19443469  0.16170528
     0.49120462  0.32631841  0.24414401  0.19496628  0.16224892  0.13891907
     0.32601349  0.24416193  0.19512628  0.16247703  0.13918004  0.12172207
     0.24372152  0.19499232  0.16248597  0.13926308  0.12184548  0.10829895
     0.19449884  0.16227878  0.13919372  0.12185049  0.10834668  0.09753552
     0.16176917  0.13895058  0.12173830  0.10830686  0.09753854  0.08871465
    CPU time 0.06   Elapsed time 0.70
    
    EXIT
    
    .DIR INVERS*
    
    
    INVERS  FOR     7  <057>   19-Mar-86    DSKB:   [10,7]
    INVERS  REL    16  <057>   19-Mar-86
      Total of 23 blocks in 2 files on DSKB: [10,7]
    
    .