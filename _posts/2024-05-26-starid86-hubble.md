---
layout: post
title: "starid86 hubble"
categories: aerospace starid
---
[paper 1986](https://statespace.dev/docs/papers/1986%20groth.pdf){:target="_blank" rel="noopener"}

the hubble space telescope played an important role in project starid, and this paper is part of that story. project starid began in 1990 as part of work with the hubble space telescope astrometry team, and the related texas minor planet project at utaustin. part of that work was based explicitly on the starid algorithm in this paper, published four years earlier by a member of the hst program on the east coast.

as a result, this paper has special significance as one of the earliest elements of project starid. and because it captures the astronomical influences in a concentrated form, thirteen years before serious aerospace influences entered the story. instead of an isolated discussion of the paper, it's possible to explore exactly how the algorithm was used for hst astrometry and the tmpp, and in the context of the beginnings of project starid. 

the starting point for the normal workflow was a photographic glass plate taken with the cass-cam on the eighty-two inch telescope at mcdonald observatory. a plate was roughly the size and shape of a piece of writing paper, as thick as window glass, and stored in a white paper sleeve with a printed form on the front for observer's notes, filled out by hand.

the dark patches of emulsion on the plate, formed by bright stars, were digitized first. the pds microdensitometer was used to raster scan these shapes, resulting in a set of digital images, one for each scanned star. all of the images were measured in the pds x, y coordinate frame. there was no point in scanning regions of the plate between stars. only bright stars were scanned, all in the global pds coordinate frame. the second step was to centroid each image, resulting in a list of x1, y1 pds star positions.

one of the observer's notes on the plate sleeve was the position on the sky of the guide star used for telescope tracking during the plate exposure, allowing a list of nearby bright star positions to be extracted from a star catalog, this is the exact opposite of the lost in space problem, and instead of having to search the entire sky for stars matching those on the photographic plate, a list of x2, y2 catalog star positions was immediately available.

now there were two lists of star positions, in different coordinate frames and with different numbers and orderings of stars. the challenge was to match some of the stars between the two lists. this was a common task for astronomers, performed manually throughout the decades of reliance on photographic plates. the new element becoming practical with eighties minicomputers and workstations was automation of the matching process.

the first step of the paper's algorithm is to make the lists roughly twenty stars long, picking the brightest first. exact similarity of the two lists is not practical, but ideally they now contain approximately the same stars and the major differences between them are ordering and coordinate frames. the objective is to match at least some, and preferably most, of the stars between the two lists. with two or more matches, a coordinate frame transformation between the two lists can be estimated.

now the same operations are performed on both lists. triangles are formed for all possible triplets of stars in a list. two features represent each triangle - the ratio of the shortest to the longest side, and the angle between them. both features are independent of coordinate frame, they don't depend on scale or orientation.

direct matches can now be made between the lists of triangles with similar ratios and angles. if a star from the photographic plate list appears in many matching triangles, its matching star in the catalog list can be determined by set operations - union, intersection, etc. numbers of matching triangles essentially 'cast votes' on star matches. if many triangle match votes are cast for a star, then it's been identified.

