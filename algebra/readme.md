
target is using python, matlab, fortran10, macro10 in parallel to reproduce [feynman's table 22-3](https://www.feynmanlectures.caltech.edu/I_22.html)

   1.00000    0.00225
   0.99999    0.00450
   0.99996    0.00900
   0.99984    0.01800
   0.99935    0.03599
   0.99741    0.07194
   0.98965    0.14350
   0.95881    0.28404
   0.83865    0.54467
   0.40666    0.91358
  -0.66925    0.74303
  
# fortran10

with fortran10, the basic workflow for iterating between raspi and tops10 [is there now](../sec5-minimalist-walkthrough.md). what would be nice is to be able to at the least do 'initial work' on fortran10's old school fortran iv/66 source code in a raspi ide debugger. clearly nothing like this will be possible for macro10, but for fortran10 it's worthwhile.

onboard the raspi, insure that 'sudo apt install gfortran' and 'sudo apt install gdb' are go. these cover the gfortran compiler, which does seem able to handle the source code. vscode and its ['modern fortran' extension](https://fortran-lang.org/) also work alright. for vscode run configurations, see [tasks.json](../../.vscode/tasks.json) and [launch.json](../../.vscode/launch.json). note macos blocks gdb so don't waste time trying fortran iv on mac.

note in the write (6, 10) the '6' is a 'unit designation' and the '10' is a format statement line number. currently unit designation for terminal is 6 on raspi but 5 on tops10. can alter the runtime env with export GFORTRAN_STDIN_UNIT=6 and export GFORTRAN_STDOUT_UNIT=5.

# macro10

with macro10 need to take an accumulator containing a floating point number and printing it on the terminal in something like f10.5 format. printing floating point is a modification of decimal out, bringing in the pdp10 floating point instructions. there's a numerical value to be printed out to the terminal, and that's done one character at a time using the 'outchr' tops10 muuo. this is a request to tops10 to print a character on the terminal, and it's used repeatedly to print each character of the numerical value, including negative sign and decimal point. 

for feynman's table 22-3, the need is to print five digits after the decimal point. that's all the code around outchr does. at the start, accumulator 'a' contains a float that is less than one. it's 'after the decimal point'. the machinery repeats five times, cranking five digits out 'in front of the decimal point', one by one, to print them with 'outchr'.

the code in algebra.mac works but something interesting happens from about the sixth or seventh of the eleven iterations. it's using standard single precision pdp10 floating point, so 27 bits of precision. it's possible that double precision would cure the problem. if this turns out to be true, it will be an excellent example of real-world effects of floating point precision, and will mean that feynman's table 22-3 is an excellent detector of poor numerical precision.

