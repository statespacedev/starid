---
layout: post
title: "starid06 uncalibrated"
---
[paper 2006](https://statespace.dev/docs/papers/2006%20samaan.pdf){:target="_blank" rel="noopener"}

this paper concisely discusses many of the standard assumptions about star tracker optical image formation. in astronomy and physics, these assumptions are often captured in the terminology 'pinhole-camera model', but it's unusual to see it mentioned in the star identification context. one of the curious things about this is that it's the direct link between between the astronomical and aerospace approaches, or equivalently the two-dimensional image and three-dimensional unit vectors approaches.

the pinhole-camera model has three parameters. focal length, x offset, y offset. the term 'uncalibrated' in the paper's title refers to high uncertainty in these three values. when the focal length and offsets are estimated from identified stars, the uncertainty is reduced. the point of this paper is that stars can still be identified regardless, and then immediately used to perform the calibration.

the foundation of the model, implicit within the three parameters, is that the star unit vectors in the camera frame are simply the 'negatives' of those in the celestial frame. the camera frame is the 'inversion' of the celestial frame, with the origin at the pinhole. every unit vector is projected through the origin to the diametrically opposite unit vector. all unit vector components are reversed. if this verbal description is confusing, the simple diagram in the paper should clarify things.

another diagram in the paper illustrates the effect of the three parameters on a star triangle. a change of the focal length scales the triangle's size in the image, and changes in the two offsets shift the triangle's position. for star identification, it's preferable that accurate unit vectors for the triangle's three stars be extracted from the image. this can be a weakness here, as for an uncalibrated camera it can be difficult to accurately extract the unit vectors. 

instead of using unit vectors for the initial identification, the paper describes a 'non dimensional' algorithm. it's based purely on two-dimensional image coordinates. regardless of the triangle's scaling in the image, the angles between the three triangle sides remain the same. the algorthm performs a direct match based on two angles. between sides one and three, and between sides two and three. the sides are ordered so that the angle between sides one and three is the triangle's largest angle, and the angle between sides two and three is the triangle's smallest angle. in other words, a direct match is performed on the largest and smallest angles.
