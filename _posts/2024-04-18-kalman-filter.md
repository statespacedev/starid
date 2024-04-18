---
layout: post
title: "kalman filter"
categories: aerospace kalman
---
as both intercontinental missiles and thermonuclear weapons became practical in the mid fifties, one more technology became essential - self-contained guidance systems that could reliably get near enough to a spot on the other side of the globe. what was needed was an autonomous digital system for guidance and control of long range missiles. this was an important motivation behind the appearance, at the beginning of the sixties, of the kalman filter.   

the kalman filter's role was to improve real-time guidance based on available sensors, preferably only onboard sensors with minimal external input. the ideal was unjammable inertial 'black-box' guidance. to achieve this, kalman introduced two explicit numerical models - one for the sensors and their uncertainties, the other for the system's dynamics and its uncertainties. 

uncertainties were represented by covariance matrices, quadratic forms with 'squared units'. the same thing appeared in finance via markowitz portfolio theory at the same time. it would be interesting to know how much contact there was between kalman and markowitz, or whether it was simply an effect of the availability of digital computers and fortran. this also took place only a few years after dantzig and von neumann's work with the simplex algorithm, and it's possible connections can be found between the whole group.  

because of the squared values, covariance matrices can contain large or small values. this was a problem for early digital computers with relatively few bits in their floating-point representations, for example the 16-bit apollo guidance computer. 'factorized' forms of the kalman filter soon appeared so that only something like 'square-roots' of covariance matrices needed to be represented within the primary computations. 
 
