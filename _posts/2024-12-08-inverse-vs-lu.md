---
layout: post
title: "inverse and lu factorization"
---

from the initial matrix code in the early fifties there's been a divide between code that works with the inverse and code that works with the lu factorization. there's an association of the former with simplex, economics, operations research, systems analysis. and there's an association of the latter with engineering, especially mechanical and aerospace. there's also a corresponding association of the former with sparse matrices, and the latter with dense matrices.

within the simplex world, shorthand for the divide is pfi and efi. product form of inverse and elimination form of inverse. interestingly, there doesn't seem to be a similar shorthand outside of the simplex world. as forsythe and moler note in their 1967 book, _unless the reader has explicit need for the elements of the inverse, we recommend strongly against computing it. as we have said, almost anything you can do with the inverse can be done without it._ on the topic of simplex they note _requires quite different attacks and for historical reasons we will not discuss them here. they are treated in textbooks on like linear programming, like dantzig 1963._

there's something special about simplex that's probably behind this divide between 'pfi and efi'. simplex is an iterative search, with each step replacing a column vector. because of the sparsity, there can be motivation to do this to the inverse rather than the lu factorization. dantzig started out with the inverse from around 1950. switching to the lu factorization seems to be associated with markowitz from around 1955.
