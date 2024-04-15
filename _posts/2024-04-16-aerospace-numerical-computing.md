---
layout: post
title: "aerospace numerical computing"
categories: aerospace numerical
---
this is an initial experiment with bringing some legacy 'blog style discussion' from python sphinx forward into website jekyll.

some of the topics of interest for this project are the early generations of computing hardware, and what became known as software, as computing migrated from mechanical to digital systems. what the room-sized machines in the old film clips were actually doing, and how fortran become so influential. part of the answer is large scale linear algebra, so floating-point matrix and vector operations, the simplex algorithm, dantzig and von neumann, and operations research.

one of the things kalman added to the picture was noncertainties, especially the classic covariance matrix quadratic form. covariance matrices appeared simultaneously in finance via markowitz portfolio theory, and this seems to have been a widespread result of increasing floating point operations and the ability to harness them via fortran. 

with the early integrated circuitry for the apollo guidance computer and minuteman missile guidance system, adaptive digital control was becoming a reality, but things were definitely on the edge of possibility. quadratic forms blew up the numerical ranges, generating numerical instability. numerical values simply became too large or small, and there weren't enough bits in the floating point representations. 

applied mathematics and electrical engineering reacted with new hardware and code for factorizing the matrices. this meant working with something similar to the square roots of the covariance matrices, at least internally. names like thornton and bierman may not be widely remembered today, but without the factorized form 'square root' ekf, the apollo guidance computer wasn't possible at the time. what are more widely known are the descendants. ud decomposition, cholesky decomposition, singular value decomposition.
