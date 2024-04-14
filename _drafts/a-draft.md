---
layout: post
title: aerospace numerical computing
categories: aerospace numerical
---
this is the first trial of bringing legacy 'blog style discussion' from statespace sphinx forward into website jekyll.

interesting and important applications of computing in the 40s, 50s, and 60s - stuff that drove forward the early
generations of hardware, and what eventually became known as software - what some of the machines in the old film
clips were actually doing, and why fortran was a big deal - tons of large scale linear algebra - floating-point
matrix and vector operations.

for the purposes here, the beginnings were dantzig and von neumann linear programming - all woven through operations
research around the time of the transition from mechanical to digital computing - early cold war. one of the things
kalman then brought in on top was nonlinear elements capable of representing uncertainty - the classic covariance
matrix quadratic form ATAx. this was happening across fields, as the same appeared in finance via markowitz portfolio
theory at the same time - what was driving all this organic evolution was the increasing floating point computing
power - flops - and the ability to harness it via fortran. and the impact was immediate - high cold war - late 50s
early 60s - with the early integrated circuitry of the apollo guidance computer and minuteman missile guidance
system. how do life-or-death automated control systems make better decisions? the birth of machine learning -
adaptive closed-loop feedback control.

increasing flops and fortran drove the appearance of covariance matrices in what was purely linear optimization - but
things were definitely on the edge - quadratic forms immediately blew up the numerical ranges and numerical
instability. there weren't enough bits in the floating point representations - plain and simple. what immediately
happened was a cottage industry within applied mathematics and electrical engineering - optimize the fortran and
floating point units, and factorize the matrices. what does factorize the matrices mean? in a nutshell,
only represent something very much like their square roots - at least internally within the processing. this all
dominated the 60s, 70s, and 80s - high cold war onward through baroque/neo-classical cold war - the era when the name
'cray' inspired awe. one of the objectives here is a straightforward minimalistic representation of the real-world
factorized forms, alongside the classic textbook forms. names like thornton and bierman aren't widely remembered
today - but without the factorized form 'square root' ekf, the apollo guidance computer wasn't possible at that time
- and they were crucial for the transit/gps satnav systems as well. those who know about ud decomposition,
cholesky decomposition, singular value decomposition, etc - know.
- 