---
layout: post
title: "decomp and invhil"
---

project decomp is about using python and fortran10 to reproduce the decompose, solve, and improve functions central to the book computer solutions of linear algebraic systems, forsythe and moler, 1967. decomp follows on from project algebra, which uses python, fortran10, macro10 to reproduce table 22-3 from feynman's algebra chapter.

the book is small and dense. it's natural to read repeatedly, and in some sense, directly comparable to feynman's chapter. deep reading and understanding is a personal turning point. possibly only reachable after four or five decades of seasoning.

both the book and feynman's chapter are concerned with algebra. the book focuses on multiple equations and limited precision. matrix operations and floating point round off for binary digital hardware. at the time, thirty-six bit single precision floating point. twenty-seven bit fractional part, eight bit exponent, one bit sign.

decomp.py first calculates an important six by six matrix 'truth' to use as an example for demonstrating the decomp, solve, improv functions. this matrix is useful because it's the inverse of a known matrix, and it contains only integers. it's a kind of 'ideal inverse matrix' for studying the algorithmic mechanisms making up the real code and hardware. by inverting 'truth', it's expected to get the known matrix back. and by inverting a second time, it's expected to get 'truth' back again. 

in reality, rounding errors enter into both of these steps. and the underlying known matrix is poorly conditioned, so both steps are particularly sensitive to rounding errors. it's something like a 'worst case' situation, and therefore particularly useful for study.
