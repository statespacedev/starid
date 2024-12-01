---
layout: post
title: "thirty-six bit floating point"
---

project decomp's fortran iv code is from the era of single precision floating point on [thirty-six bit hardware](https://en.wikipedia.org/wiki/36-bit_computing). in particular, the example results in the book are from thirty-six bit calculations. twenty-seven bit fractional part, eight bit exponent, one bit sign. this is associated with roughly eight significant decimal digits.

to roughly align with modern hardware, thirty-two bit floating point has to be carefully enforced. there's still a difference of four bits some practical effects are expected. here's info on modern thirty-two bit.

_In the [IEEE 754](https://en.wikipedia.org/wiki/Single-precision_floating-point_format) standard, the 32-bit base-2 format is officially referred to as binary32; it was called single in IEEE 754-1985. The IEEE 754 standard specifies a binary32 as having: Sign bit: 1 bit, Exponent width: 8 bits, Significand precision: 24 bits (23 explicitly stored). This gives from 6 to 9 significant decimal digits precision._

the effective difference is three or four bits in the significand, depending on what exactly is meant by 'explicitly stored'. relative to roughly eight significant decimal digits for twenty-seven bits, this suggests the effects will be limited to the least significant one or two decimal digits. six or seven significant digits should match between old and new. 

some of the effects are somewhat indirect and subtle. rounding errors will build up quicker with increasing flops. with four fewer bits in the significand, floating point normalization will 'degrade' the least significant bits quicker. normalization means shifting, and four more bits will be shifted into noise / entropy, effectively losing four bits on many ops. there's also greater sensitivity to subtractions involving similar values. these give results in just the least significant bits, where there are four fewer.
