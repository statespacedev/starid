---
layout: post
title: "modern kalman filter"
categories: aerospace kalman
---
the modern kalman filter is an intermediate step between the classical kalman filter, where uncertainty is represented as gaussian, and the particle filter. the advantage of gaussians is that they have only two parameters, so asked the bare minimum from late fifties computing hardware. much effort went into efficiently evolving the values forward through time via the matrix riccati differential equation for the covariance.

the step forward from the classical kalman filter was to deterministically sample the uncertainty and represent it as a small number of points, the sigma points or sigma particles, surrounding an expected point. later, with the particle filter, the sampling points or particles became more plentiful and non-deterministic. large numbers of particles can build up arbitrary uncertainty 'shapes'. 

in the seventies and eighties, integrated circuits were becoming common and floating-point operations were becoming relatively fast. 'sample-and-propagate' algorithms such as the modern kalman and particle filters became practical with the increasing computing power.

the sigma points are associated with 'sigmas' in the sense of 'square-roots of variances'. they form an ellipsoid of uncertainty around the expectation, representing a 'one-sigma surface'. there's roughly a two-thirds probability that the true point is somewhere inside the one-sigma surface.
