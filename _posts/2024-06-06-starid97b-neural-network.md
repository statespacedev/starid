---
layout: post
title: "starid97b neural network"
---
[paper 1997b](https://statespace.dev/docs/papers/1997%20lindblad.pdf){:target="_blank" rel="noopener"}

related discussion in an [earlier post](https://statespace.dev/nineties.html)

this paper is tied to a neural network chip, the ibm zisc036 'zero instruction set computer', named in direct opposition to risc 'reduced instruction set' and cisc 'complex instruction set' hardware designs. [zisc hardware](https://en.wikipedia.org/wiki/No_instruction_set_computing#Zero_instruction_set_computer) is purely about pattern recognition using a single parallel 'wide operation', as opposed to a long sequence of 'narrow operations'.

the original concept appears to have been recognition of a target star using three other measured stars. this is a pattern of two triangles joined along a shared side, first described in the starid89 paper. on top of that, two star brightnesses were included. pattern characteristics were extracted and encoded into the standard ibm zisc input vector format. the pattern characteristics are inherently insensitive to star tracker yaw / image rotation.

recognition performance with this original approach was poor. the paper explains how this was due to measurement noise, which is surprising since 'fuzzy' resistance to noise was one of the selling points for neural network chips. possibly the star tracker was too much on the 'lo-fi' end of the spectrum and/or the neural network chip was too much on the 'hi-fi' end. in any case, the paper moves onward to experiments with several increasingly simple approaches. the description of that process is a useful review of the possibilities.

first the use of star triangles was abandoned, and the focus shifted to simply angular separation from a target star and brightness. a plot of brightness versus angle seems like a viable signature for a target star. given enough measured stars, it should be a fairly unique and recognizable pattern for matching.

in the end, brightness was abandoned as well. the simplest viable approach was a list of angular separations from a target star. again, given enough measured stars it should be a usable pattern for recognition. the downside is that much of the information coming from the star tracker is discarded along the way. but this could have advantages of its own at the 'lo-fi' end of the spectrum. these simplified patterns are something to be explored in project starid.
