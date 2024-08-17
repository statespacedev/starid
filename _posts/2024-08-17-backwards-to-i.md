---
layout: post
title: "square root of negative one"
---

a wheel is turning around its axle at a steady rate. the wheel's radius is one and a spot on its edge has coordinates x = cos(t) and y = sin(t). the distance of the spot from the axle is x^2 + y^2 = 1. 

rather than represent x and y separately, can they be somewhat unified? for example, a tuple (x, y). or a vector. or, even better, a unit-vector. or an arithmetic expression. one possibility is x + k * y. the factor 'k' can be plus or minus itself and x - k * y is in some sense equivalent, and leads onward to writing down and contemplating some algebra.

    (x + k * y)(x - k * y) = x^2 - k^2 * y^2 

if this expression equals the wheel's radius of one, then 'k' is the square root of negative one, and the symbol 'i' has appeared as a scaling factor that 'makes the geometry work'.

this is a 'backwards' approach to what feynman is discussing towards the [end of this chapter](https://www.feynmanlectures.caltech.edu/I_22.html). basically, starting at the end of the chapter and 'reverse engineering' back towards the beginning.

to start at the beginning and go forwards instead of backwards, what feynman did was to gradually and step-by-step get to an imaginary exponent. in particular, ten to the 'i' times 's', 10^is. he then noted that 10^is = x + i * y, and that 10^-is = x - i * y. he was then able to write some algebra.

    (x + i * y)(x - i * y) = x^2 + y^2 = 1

from there, he soon arrived at the final destination, after changing the base from ten to 'e'.

    e^it = cos(t) + i * sin(t)
