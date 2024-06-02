---
layout: post
title: "starid89 sirtf"
#categories: aerospace starid
---
[paper 1989](https://statespace.dev/docs/papers/1989%20bezooijen.pdf){:target="_blank" rel="noopener"}

in the seventies and eighties, nasa initiated two high-level scientific programs, both of which have played central roles in the project starid story - the [great observatories program](https://en.wikipedia.org/wiki/Harlan_James_Smith) looking outwards, and the [earth observing system](https://en.wikipedia.org/wiki/Earth_Observing_System) looking inwards.

the hubble space telescope directly entered the starid story first, in 1990. it was the flagship of the great observatories program and well-known and loved by school-kids of the eighties. it's sibling, the [space infrared telescope](https://en.wikipedia.org/wiki/Spitzer_Space_Telescope) was less well-known, but important. 

this paper summarizes the star identification work done in the eighties for the sirtf attitude control system. the algorithm was also extended to a commercial star tracker, where the scope grew to include the lost in space problem. the result seems to be that the 'hi-fi' end of the star identification spectrum was targeted. in particular, both star brightness and position measurement accuracies play a prominent role. the direct relevance for the 'lo-fi' end of the spectrum is questionable.

the indirect relevance is strong however, as this paper seems to be first where two triangles with a shared side are diagrammed and discussed directly. an intuitive diagram of the situation has four stars as the four corners of a shape like a children's kite, with the two support sticks of the kite dividing the shape into four quadrants. this paper seems to be the first appearance of this type of diagram. the two support sticks are the shared sides of two overlapping and complementary pairs of triangles.

in this case, the shared sides are essentially used as secondary validation checks after the primary star identification algorithm. they aren't actively used as part of determining star identities, but are instead used to confirm that identities are valid. the primary determination of star identities uses methods similar to earlier papers, based on non-iterative direct matching of accurate star pair angles and brightnesses. the 'shared side validation' is an additional operation added on top of that.


