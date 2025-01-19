---
layout: post
title: "maintain the basis inverse"
---

the alp book makes clear the nature of the breakthrough in coding and computing that took place at rand santa monica around 1953. before, simplex had focused on a table or picture of numerical columns representing a problem. this table was what a human would naturally work on using pencil, paper, and desktop calculator. it was an accounting ledger and a human accountant performing a search of various 'table column combinations'. the unfortunate term 'tableau' refers to all of this.

the real problem was that this 'traditional accounting' approach had poor numerical accuracy when implemented on a digital computer. it was difficult to maintain the table accurately over many iterations because of numerical issues. 

the solution was to 'maintain the basis inverse', instead of 'maintaining the table'. switching to this approach created the 'revised' simplex. instead of working in and maintaining a 'direct problem space', revised simplex switched over into an 'inverse problem space' and maintained that. this was a familiar strategy in aerospace computing as well.

at first, 'maintaining the basis inverse' meant the 'product form of the inverse', later the 'lu decomposition'. lu decomposition was becoming standard, particularly in the world of dense matrices associated with engineering, the las book, forsythe, moler, etc. early simplex and the pfi direct 'naked inverse' matrix is somewhat special.

to be realistic for fifties simplex, the pfi has to be the focus. it was the standard well into the sixties. this was the orchard hays era. simplex characteristics such as sparsity, column vectors and column major representations, and iterative column search, with columns entering and leaving the basis matrix, made the pfi reasonable and even 'natural'.

a sure sign of the pfi are the 'eta vectors'. an eta vector is a column of the basis inverse, and represents an 'elementary column matrix'. the basis inverse is composed of elementary column matrices, hence of eta vectors. this explanation makes clear how pfi is simply 'another decomposition', alongside decompositions such as lu.

for fifties hardware, eta vectors were ideal because they were a natural fit for magnetic tape. there wasn't enough random access memory, generally magnetic core. so tape was used as a 'slow memory'. the eta vectors, and hence the basis inverse, were kept on tape. the famous BTRAN and FTRAN stages of the revised simplex iteration accessed the tape eta vectors 'backwards' and 'forwards', respectively. in old films with scenes set in computation centers with rows of hulking tape drives, many of those spinning tapes are probably doing BTRAN and FTRAN. dr. strangelove is an excellent example. group captain lionel mandrake has several closeups with an ibm 7090 and tape drives, probably doing BTRAN and FTRAN.
