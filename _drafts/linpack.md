---
layout: post
title: "linpack"
---

nsf funding at argonne national lab led to the fortran linear equation package linpack by the early seventies. it's closely tied to supercomputing and is the standard for [supercomputer benchmarking](https://en.wikipedia.org/wiki/LINPACK#World's_most_powerful_computer_by_year), from cdc and cray in the sixties and seventies through today.

[cleve moler](https://en.wikipedia.org/wiki/Cleve_Moler) and his advisor [george forsythe](https://en.wikipedia.org/wiki/George_Forsythe) at stanford in the sixties are of special interest in the linpack context. here's some comments from moler on 'computer solutions of linear algebraic systems', 1967, forsythe and moler

_this is a little book, it’s only less than 150 pages ... solving systems of simultaneous linear equations is arguably the most  important computation in scientific computing, it underlies everything else ... if you were let’s say studying control theory or finite element computations in engineering at this time, you would have used this subroutine from this book in your own program_

the mainframes in their computation centers now had a portable software standard. fortran66. the linpack tapes from argonne spread rapidly. it was a milestone in the history of 'open source and free software'. matrix and vector code was in demand at every engineering and scientific site. the tapes arrived and the users got access via their serial-line terminals.

by the early eighties moler had made linpack even more accessible as matlab. he noticed a split among matlab users. roughly half, the math and computer science majors, were disappointed with matlab's limited features. the other half, engineering majors, liked it.

_they were studying subjects that i didn't know anything about, such as control analysis and signal processing, and the emphasis on matrices in matlab proved to be very useful for them._

here's a [good review](docs/inverse/1966%20forsythe.pdf) by forsythe of the state of the art in the fifties and sixties, particularly around matrix inversion.
