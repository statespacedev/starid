---
layout: post
title: "six easy pieces"
---

forsythe's review of [linear algebra's state of the art in the fifties and sixties](docs/inverse/1966%20forsythe.pdf) is a nice bit of nostalgia. six core topics are discussed. the first five, up through least squares, match perfectly with a linear algebra course in summer 2001. the sixth, simplex, with an astronomical data analysis course a few years later. those were some of the most positive experiences with math as a subject in its own right, and the fact that it's all on the extreme applied end of the spectrum is no accident.

the first two are closely related. solving simultaneous linear equations, and the inverse matrix. there's a variety of routes to solutions, for example the mechanistic 'gaussian elimination' taught and performed by hand in courses. the author comments that even when the inverse appears, it's usually more practical to deal only with its parts, rather than the full and complete matrix.

the second two are eigenvalues and vectors for symmetric and non-symmetric matrices, in others words solution finding for simultaneous equations. it's unfortunate that the jargon mystifies things, even awkwardly wedging in some germanic murk. this is algebra and finding solutions. here vibration, resonance, and stability are mentioned, with for example flutter in aircraft and criticality of reactors. the algorithms orbit around 'difference equations' / 'finite difference methods', which are the discrete / non-continuous / digital computing way to deal with the continuous / analog world's 'differential equations' / 'infinitesimal difference methods'. 

the fifth and sixth are least squares and simplex. here concepts of uncertainty and optimization are increasingly important, and the reader is referred onward to golub or dantzig. least squares and simplex are reaching out to the complexities of the real world. they're making a leap forward, and maybe even a bit too applied for the author. in any case, he focuses on the first four and doesn't say much at all about simplex beyond mentioning linear programming and dantzig's name. possibly one distinction is that simplex is concerned with sparse matrices, whereas the author is concerned with dense matrices.

on the other hand, he mentions von neumann quite a bit, especially in the context of matrix inversion. and even that von neumann's work on the topic was based on fixed point arithmetic. no floating point. here's a curious comment concerning the early days, circa 1953.

_in any case, it could only handle scaled fixed point arithmetic. because of the size of their error bounds, von neumann and goldstine were unnecessarily pessimistic about the possibility of inverting general matrices of orders over fifteen on machines with the twenty-seven bit precision of the ibm 7090._
