---
layout: post
title: "starid04 pyramid"
---
[paper 2004](https://statespace.dev/docs/papers/2004%20mortari.pdf){:target="_blank" rel="noopener"}

an aerospace approach is explicit in this paper, with three-dimensional unit vectors throughout. this differs from earlier papers with a more astronomical approach and uneasy mixtures of two-dimensional image coordinates and occasional spherical geometry. 

in a major break from earlier work, star brightness measurements are explicitly excluded from the algorithm. pure geometric shapes are used, and this can be seen as another aspect of moving away from astronomical traditions. if star brightness is not needed, or is even an active hindrance to robustness, then why use it.

two triangles with a shared side are referred to as a 'pyramid'. this could make sense in the context of looking downward from above at a three-dimensional pyramid resting on the ground. possibly it's helpful to visualize the four stars making up the two triangles as the corners of a pyramid's base, with a fifth 'virtual' star at the pyramid's peak. another possibility is a visual illusion where a sketch with six lines connecting four stars simply 'looks like' a wire-frame diagram of a three-dimensional pyramid seen at an angle.

the phrase basic star triangle and confirming fourth star is also used, and seems to be completely equivalent to both pyramid and pair of triangles with a shared side. star triangle and confirming fourth star is actually a good description of the algorithm's core. with a base triangle, different fourth stars are tried iteratively, until confirming evidence exceeds a threshold. if the threshold isn't reached, a new base triangle is chosen and the process is repeated. 

pairs take on a special importance in this paper, as a standalone pair search algorithm termed 'k-vector' is emphasised. this converts searching for pairs into something in-between a lookup table and a hash table. two indexes into the table are computed by a deterministic 'hash function'. they're the first and last indexes into the lookup table, and the search results are the table rows between.
