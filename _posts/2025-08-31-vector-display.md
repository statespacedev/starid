---
layout: post
title: "vector display"
---

a more dynamic version of this info is [here](https://gitlab.com/decwar/galaxy#vector-display)

the reference baseline here is the original sage display and its direct descendant dec 30 / 340 display. a more modern, eighties, example is tempest. note from this wiki discussion that an important factor was the cost of core memory. only vector endpoints needed storage, providing what was in some sense 'image compression'. this could be a useful definition for 'vector display' in the decwar context. only certain positions are specified / stored, ideally in an arbitrary floating-point space.

_in a vector display, the image is composed of drawn lines rather than a grid of glowing pixels as in raster graphics. the electron beam follows an arbitrary path, tracing the connected sloped lines rather than following the same horizontal raster path for all images. the beam skips over dark areas of the image without visiting their points. some refresh vector displays use a normal phosphor that fades rapidly and needs constant refreshing 30-40 times per second to show a stable image. these displays, such as the imlac pds-1, require some local refresh memory to hold the vector endpoint data. other storage tube displays, such as the popular tektronix 4010, use a special phosphor that continues glowing for many minutes. in the 1970s, both types of vector displays were much more affordable than bitmap raster graphics displays when megapixel computer memory was still very expensive._

vector displays have no direct historical connection with decwar. with modern robots playing however, a way to watch the whole galaxy is useful and vector display would be fun. there's at least an indirect connection, as trek / decwar grew from around 1971 because of the lack of a pdp-1 display. the iconic 'scan command' ascii art output, often seen via teletype as well as crt, was a practical solution for low end hardware.

to approximate some of the characteristics of a sixties vector display crt, the dec type 30 is a useful reference baseline. based on common sense, it was a direct descendant of the sage terminal crt. both were 16 inch circular tubes, both worked with light-guns, and both were created by the same people in the boston area. dec was essentially a commercial splinter from the sage project.

for a circular 16 inch tube face, the largest square has sides of length 16 inches / sqrt(2) = 11.3 inches. the type 30 had 1024 x 1024 coordinate addresses within a 9.5 inch square on the tube face. so the usable square on the display was approaching the largest square possible, with sides sacrificing roughly two inches to avoid the edges of the tube face. 

at the center of a 9.5 inch square side, the edge of the tube face was (16 - 9.5) / 2 = 3.25 inches away. this seems to somewhat roughly agree with the relative sizes of the tube face and of the 'square of dots' in this photo of a sage display. if the sage display provided the maximum 11.3 inch square, that would change to (16 - 11.3) / 2 = 2.35 inches. possibly the photo's 'square of dots' is the full 11.3 inches. the corners of the 'square of dots' do appear to be approaching the edges of the tube face.
