alter the runtime env with the following two bash commands, put em in .bashrc

    export GFORTRAN_STDIN_UNIT=6
    export GFORTRAN_STDOUT_UNIT=5

project decomp is about using python and fortran10 to reproduce the decompose, solve, and improve functions central to the book computer solutions of linear algebraic systems, forsythe and moler, 1967. decomp follows on from project algebra, which uses python, fortran10, macro10 to reproduce table 22-3 from feynman's algebra chapter.

the book is small and dense. it's natural to read repeatedly, and in some sense, directly comparable to feynman's chapter. deep reading and understanding is a personal turning point. possibly only reachable after four or five decades of seasoning.

both the book and feynman's chapter are concerned with algebra. the book focuses on multiple equations and limited precision. matrix operations and floating point round off for binary digital hardware. at the time, thirty-six bit single precision floating point. twenty-seven bit fractional part, eight bit exponent, one bit sign.

decomp.py first calculates an important six by six matrix 'truth' to use as an example for demonstrating the decomp, solve, improv functions. this matrix is useful because it's the inverse of a known matrix, and it contains only integers. it's a kind of 'ideal inverse matrix' for studying the algorithmic mechanisms making up the real code and hardware. by inverting 'truth', it's expected to get the known matrix back. and by inverting a second time, it's expected to get 'truth' back again. 

in reality, rounding errors enter into both of these steps. and the underlying known matrix is poorly conditioned, so both steps are particularly sensitive to rounding errors. it's something like a 'worst case' situation, and therefore particularly useful for study.

# thirty-six bit floating point

project decomp's fortran iv code is from the era of single precision floating point on [thirty-six bit hardware](https://en.wikipedia.org/wiki/36-bit_computing). in particular, the example results in the book are from thirty-six bit calculations. twenty-seven bit fractional part, eight bit exponent, one bit sign. this is associated with roughly eight significant decimal digits.

to roughly align with modern hardware, thirty-two bit floating point has to be carefully enforced. there's still a difference of four bits some practical effects are expected. here's info on modern thirty-two bit.

_In the [IEEE 754](https://en.wikipedia.org/wiki/Single-precision_floating-point_format) standard, the 32-bit base-2 format is officially referred to as binary32; it was called single in IEEE 754-1985. The IEEE 754 standard specifies a binary32 as having: Sign bit: 1 bit, Exponent width: 8 bits, Significand precision: 24 bits (23 explicitly stored). This gives from 6 to 9 significant decimal digits precision._

the effective difference is three or four bits in the significand, depending on what exactly is meant by 'explicitly stored'. relative to roughly eight significant decimal digits for twenty-seven bits, this suggests the effects will be limited to the least significant one or two decimal digits. six or seven significant digits should match between old and new. 

some of the effects are somewhat indirect and subtle. rounding errors will build up quicker with increasing flops. with four fewer bits in the significand, floating point normalization will 'degrade' the least significant bits quicker. normalization means shifting, and four more bits will be shifted into noise / entropy, effectively losing four bits on many ops. there's also greater sensitivity to subtractions involving similar values. these give results in just the least significant bits, where there are four fewer.

# inverse versus lu factorization

from the initial matrix code in the early fifties there's been a divide between code that works with the inverse and code that works with the lu factorization. there's an association of the former with simplex, economics, operations research, systems analysis. and there's an association of the latter with engineering, especially mechanical and aerospace. there's also a corresponding association of the former with sparse matrices, and the latter with dense matrices.

within the simplex world, shorthand for the divide is pfi and efi. product form of inverse and elimination form of inverse. interestingly, there doesn't seem to be a similar shorthand outside of the simplex world. as forsythe and moler note in their 1967 book, _unless the reader has explicit need for the elements of the inverse, we recommend strongly against computing it. as we have said, almost anything you can do with the inverse can be done without it._ on the topic of simplex they note _requires quite different attacks and for historical reasons we will not discuss them here. they are treated in textbooks on like linear programming, like dantzig 1963._

there's something special about simplex that's probably behind this divide between 'pfi and efi'. simplex is an iterative search, with each step replacing a column vector. because of the sparsity, there can be motivation to do this to the inverse rather than the lu factorization. dantzig started out with the inverse from around 1950. switching to the lu factorization seems to be associated with markowitz from around 1955.

# las and alp

project decomposition, and eventually project simplex, are 'working through' two landmark books. _computer solutions of linear algebraic systems, forsythe and moler, 1967_ and _advanced linear programming computing techniques, orchard hays, 1968_. they'll be referred to so often going forward, it's probably best to establish a naming convention early on. las for linear algebraic systems. alp for advanced linear programming.

both are concentrated summaries from first-generation coders, discussing their work from around 1950 through the mid sixties. and both concern code for the ibm machines from the cpc card programmed calculator, through the 701 defense calculator, up to the 7094. roughly speaking, the examples in both concern the 7094. it's the baseline system assumption for both. 

a few other manufacturers are mentioned occasionally, always as brief 'comparisons', and notably not including dec. the large scale matrix math of these books necessitated 'large systems', and probably the pdp-10 was just beginning to make dec viable in this field. this was an interesting twist of events, as dec was the child of project whirlwind / sage. the 701 defense calculator was also an offspring of whirlwind / sage. 

in a nutshell, dec commercialized the interactive and 'smaller scale / bottom up' aspects, while ibm stuck with the batch processed 'imperial scale / top down' aspects. eventually dec grew able to challenge ibm directly, with the pdp-10 large scale interactive system. significantly, both lineages retained the thirty-six bit architecture.

# notes

for fortran iv on linux, sudo apt install gfortran gdb, and vscode fortran-lang extension. see repo .vscode/tasks.json and launch.json. note macos is out because gdb is blocked via code signing.

in the fortran statement 'write (x, y)' the x is a 'unit designation' and the y is a format statement line number. default unit designation for terminal is 

    6 on raspi gfortrn
    5 on tops10 fortran10

the fix is to alter the runtime env on the raspi with the following two bash commands

    export GFORTRAN_STDIN_UNIT=6
    export GFORTRAN_STDOUT_UNIT=5

# 20250105 first complete run on pdp10

complete code including iterative improvement. there are interesting numerical issues on the pdp10 compared to on linux, but the code is all there and working on both 'as is'!

    .ex inverse
    FORTRAN: INVERS
    MAIN. 	
    INVERT	
    IMPRUV	
    DECOMP	
    SOLVE 	
    SING  	
    INVHIL	
    HIL   	
    PRINT1	
    PRINT2	
    LINK:	Loading
    [LNKXCT	INVERS execution]

    0.98892106  0.49116241  0.32595560  0.24365902  0.19443469  0.16170528
    0.49120462  0.32631841  0.24414401  0.19496628  0.16224892  0.13891907
    0.32601349  0.24416193  0.19512628  0.16247703  0.13918004  0.12172207
    0.24372152  0.19499232  0.16248597  0.13926308  0.12184548  0.10829895
    0.19449884  0.16227878  0.13919372  0.12185049  0.10834668  0.09753552
    0.16176917  0.13895058  0.12173830  0.10830686  0.09753854  0.08871465

    0.98970806  0.49180831  0.32650311  0.24413332  0.19485413  0.16208157
    0.49180839  0.32681585  0.24456675  0.19533370  0.16257395  0.13921057
    0.32650322  0.24456678  0.19547102  0.16277715  0.13944580  0.12196055
    0.24413344  0.19533375  0.16277717  0.13951687  0.12207039  0.10850088
    0.19485425  0.16257401  0.13944582  0.12207040  0.10854168  0.09771068
    0.16208169  0.13921063  0.12196058  0.10850090  0.09771068  0.08886933

    1.00000000  0.50000000  0.33333334  0.25000000  0.20000000  0.16666667
    0.50000000  0.33333334  0.25000000  0.20000000  0.16666667  0.14285714
    0.33333334  0.25000000  0.20000000  0.16666667  0.14285714  0.12500000
    0.25000000  0.20000000  0.16666667  0.14285714  0.12500000  0.11111111
    0.20000000  0.16666667  0.14285714  0.12500000  0.11111111  0.10000000
    0.16666667  0.14285714  0.12500000  0.11111111  0.10000000  0.09090909

         35.87     -626.44     3336.10    -7498.21     7492.17    -2745.39
       -626.39    14598.69   -87520.84   209924.59  -218571.46    82402.89
       3335.67   -87516.33   559896.62 -1399352.81  1498983.56  -577009.59
      -7496.86   209904.96 -1399299.36  3598038.41 -3935202.34  1539050.42
       7490.41  -218543.48  1498882.22 -3935092.13  4372745.81 -1731733.36
      -2744.61    82389.94  -576956.95  1538974.17 -1731697.22   692787.19

         35.90     -627.03     3339.84    -7507.44     7501.88    -2749.07
       -627.03    14614.86   -87622.13   210173.43  -218834.24    82502.80
       3339.84   -87622.12   560558.15 -1400976.59  1500697.56  -577661.17
      -7507.44   210173.41 -1400976.53  3602152.53 -3939542.69  1540699.92
       7501.88  -218834.21  1500697.45 -3939542.56  4377438.81 -1733516.33
      -2749.07    82502.78  -577661.11  1540699.84 -1733516.30   693478.09

         36.00     -630.00     3360.00    -7560.00     7560.00    -2772.00
       -630.00    14700.00   -88200.00   211680.00  -220500.00    83160.00
       3360.00   -88200.00   564480.00 -1411199.98  1512000.02  -582120.00
      -7560.00   211680.00 -1411199.98  3628800.03 -3969000.00  1552320.02
       7560.00  -220500.00  1512000.02 -3969000.00  4410000.00 -1746359.98
      -2772.00    83160.00  -582120.00  1552320.02 -1746359.98   698544.02
    CPU time 0.05   Elapsed time 5.28

    EXIT

    .

