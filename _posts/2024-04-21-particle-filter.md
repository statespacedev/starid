---
layout: post
title: "particle filter"
#categories: aerospace particle
---
from the modern kalman filter, the next step forward with 'sample-and-propagate' algorithms was to use non-deterministic sampling points or particles. a descriptive name for this is sequential monte carlo sampling processor, but a common short-hand is particle filter.

in the eighties and nineties, integrated circuits and increasing floating-point speed made particle-style 'simulation' practical in more situations. what's being simulated with sequential monte carlo is the dynamics and evolution of an uncertainty 'shape'. more floating-point speed means more particles and more detailed or 'higher resolution' uncertainty shapes.

one of the new ideas introduced with the particle filter is resampling and the evolution of improved particles. this makes it a member of the broad group of survival-of-the-fittest 'genetic algorithms'. the 'dna' concept is de-emphasized here in favor of using sensors to bring in new information from the external world, and the focus is evolution through time, rather than static optimization at a single point in time.

the methods used to 'resample' the less fit particles differentiate the various implementations, but the essential features are straightforward. when new data comes in, some of the particles agree with the data better than others. the former are fitter and are used as the parents of a new generation, the latter are less fit and a mechanism is used to 'cull the herd'.

for a state vector with x components, the modern kalman filter gives 2x + 1 particles. two sigma point particles for each state component, and one particle representing the expected state. with sequential monte carlo, there's simply a 'cloud' of particles swarming forward through time. the cloud's expected state can be estimated using kernel density methods, and likewise for the shape or scatter of the cloud and various measures of uncertainty.
