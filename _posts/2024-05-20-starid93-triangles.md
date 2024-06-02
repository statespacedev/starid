---
layout: post
title: "starid93 triangles"
#categories: aerospace starid
---
[paper 1993](https://statespace.dev/docs/papers/1993%20liebe.pdf){:target="_blank" rel="noopener"}

this has been the earliest known paper that directly tackles the lost in space problem. the author doesn't use the term 'lost in space'. this could be a result of being outside the sphere of american aerospace jargon, or it could be evidence that the lost in space problem wasn't yet widely recognized as practical.

 the topic of 'triangular features' is a clue that the lost is space problem is the subject. roughly speaking, if the lost in space problem is involved, the concept of star triangles will necessarily appear. and conversely, if star triangles don't appear, there's probably a finite attitude uncertainty involved so that they're not needed.

a trio of three-dimensional unit vectors, or equivalently their three dot products, are a fundamental representation of a star triangle. surprisingly, unit vectors and dot products aren't directly invoked in this paper. a 'spherical angle' does make an appearance, along with a pair of 'distance angles', so a shadow of three dimensionality is present. indications are that the thinking remained essentially two dimensional, concerning camera images and basic astronomical-style practices. in other words, this paper is a good example of an 'astronomical approach', as opposed to an 'aerospace approach'.

another aspect of the lost in space problem is that the runtime sky information has to cover the entire sky and be searchable as a whole. the sky necessarily has to be an indivisible whole. narrowing search to subregions of the sky defeats the purpose of the lost in space problem, which is to cope with an infinite attitude uncertainty. 

what this paper does is build a searchable set whose members are triplets of floats. each of the the floats in a triplet is a dot product. the first is between stars a and b. the second and third are between stars b and c, and stars c and a. star b is the 'nearest neighbor' to star a, and star c is the 'second nearest neighbor'.

this effectively increases the 'hi-fi' high resolution requirements on the star tracker and the measurements it feeds into the starid algorithm. essentially, the 'nearest neighbor' and 'second nearest neighbor' have to be highly accurate and reliable. this is the opposite of the 'lo-fi' objectives of project starid, where the star tracker and its outputs are as primitive as possible, on the level of a [children's camera from the eighties](https://en.wikipedia.org/wiki/PXL2000). lo-fi forces the algorithms to work with global features, rather than high accuracy localized features such as ordering of neighbors and accurate numerical values. 

for a concrete example, assume the true triplet of floats for a star in some artificial units is (2173.89, 937.49, 2366.03). the measured triplet from a hi-fi star tracker is (2173.7, 937.6, 2366.0). for a lo-fi star tracker, with pixelation 'rounding' effects, the measured triplet is (2, 1, 2). the lo-fi measurement might be effective for distinguishing the star's signature as long as there's only a handful of other stars, but as the number of stars grows, more stars will have the same lo-fi signature. the lo-fi star tracker forces recognition algorithms to use additional information.
