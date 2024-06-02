---
layout: post
title: "starid97a grid algorithm"
#categories: aerospace starid
---
[paper 1997a](https://statespace.dev/docs/papers/1997%20padgett.pdf){:target="_blank" rel="noopener"}

the introduction section of this paper has an interesting discussion of types of star identification. it divides algorithms into two categories. the first iteratively eliminates candidates based on geometry, usually triangles, until only one remains. the second makes a direct match or lookup of some 'feature' extracted from an image.

direct match has the advantage of not depending on iteration, since iteration takes time. ideally, direct match is essentially a single lookup, hash-table, key-value style operation. there are usually tradeoffs however, and a possible downside for direct match is an increased dependence on star tracker quality. 

iterative elimination is possibly more robust for lo-fi starid, while hi-fi starid makes direct match increasingly practical. the lower the fidelity, the more similar any extractable image features become. it's an interesting topic for project starid to pursue over time. since the focus is on lo-fi starid, it would be interesting to arrive at a rough spectrum of algorithms, from lo-fi to hi-fi, as an aid to concentrating effort on the lo-fi end of the spectrum.

the 'grid' algorithm of the paper title is similar to how project starid resamples to the 38 x 38 pixel mnist database standard. 'downsampled' and 'quantized' would also be good names. the special thing the paper does is use the nearest neighbor
star to rotate the image to a deterministic zero yaw or orientation, and this is exactly what makes direct match possible in this case. the rotated images have a common 'gravity' or 'up and down', namely, the nearest neighbor star. 'artificial gravity' could also be a possible replacement for the name 'grid'.

with a downsampled zero yaw image, the paper simply extracts an ordered-list vector of pixels containing stars. this is the feature, and it's ideally matched with a key in a key-value store of sky information. 
