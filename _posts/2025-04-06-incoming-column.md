---
layout: post
title: "simplex incoming column"
---

simplex incoming column choice [is now working](https://github.com/statespacedev/starid/blob/d563ccd99a56dc5dcc461081e67893a3b881b12f/simplex/smpl5.m). 

a new column replaces an old column of the basis during each simplex iteration. these are the incoming and outgoing columns, and simplex has to make two decisions. the cycle is repeated until a halting condition is met, and is recognizable by the signature 'btran and ftran' functions. 

there are interesting parallels with the feedback control algorithms created at the same time as simplex. the classic propagate and update phases of the kalman filter cycle at least roughly match up. the parallels between least squares and linear programming are clear from a purely linear algebra perspective, and in some sense the duality formulations shift back and forth between the two. a simple dimensional analysis reveals this. more rows than columns is least squares, more columns than rows is linear programming. how strongly this symmetry flows onward into sequential least squares / kalman and simplex is a curious topic, to be explored later.

it took effort to figure out how to automate the incoming column choice. the description in alp advanced linear programming is spread over two chapters and mixed with mathematical proofs. the mid sixties printing in the physical book is a challenge. the font is small. the mathematical notation is faint and somewhat unusual, with a locally defined symbology for row and column identifiers. once the habit is acquired, it's effective enough. 

as a rough estimate, it required two or three times more rereadings than with the forsythe moler las linear algebraic systems book, before comprehension really started to seep in. maybe five to ten total rereadings of crucial sections. implementing and seeing it working in code was essential. in the end, it's machinery for choosing an incoming vector. actually building and using the machine was where enlightenment was achieved.

one of the complexities is that the machinery needs to work in two very different environments. both when the equations are infeasible, during early iterations, and once they're feasible but not yet optimal. the way that happens is via a row vector that orchard hays terms the 'pricing form f'. probably a more modern and understandable name would be the 'pricing mask f'. it's a mask for picking out certain equations / rows by weighting with -1, 0, or 1. after setting an appropriate pricing mask, the remaining code improves both feasibility and optimality. this is impressive, especially considering that it was created in the mid fifties, probably directly in machine code. for orchard hays, assembly was the nextgen tool. there's no sign that he ever combined fortran with assembly.

btran then transforms the 'pricing form' row vector f into another row vector, the 'pricing vector' pi, by post multiplying by the basis inverse, represented by the eta vectors on tape. the pricing form mask effectively uses certain rows from the basis inverse. this is clear when the equations have already become feasible. there's a single 1 in the pricing form and btran uses one row of the basis inverse as the pricing vector.

the price of each column in the original tableau is its dot product with the pricing vector. prices are computed for the columns not in the basis or at bound. the column with the most negative price is the best to bring into the basis. it's chosen as the incoming vector.  
