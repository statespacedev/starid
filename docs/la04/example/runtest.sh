#!/bin/sh
#
# This script is used to run automated tests when "make check" is run.
#

PROG=$1
# either '' or 'mpirun -n x'
MPI=""

if [ -f $srcdir/$PROG.data ]
then
   $MPI ./$PROG < $srcdir/$PROG.data > $PROG.log
else
   $MPI ./$PROG > $PROG.log
fi
diff $PROG.log $srcdir/$PROG.output
