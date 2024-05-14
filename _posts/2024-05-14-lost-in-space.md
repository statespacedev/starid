---
layout: post
title: "lost in space problem"
categories: aerospace starid
---
the core of the lost in space problem is a total lack of star tracker pointing knowledge. often there's at least some knowledge of the star tracker's attitude, but the lost in space problem is the extreme opposite case, with infinite attitude uncertainty.

the terms 'pointing' and 'attitude' are used interchangeably here. yaw around the pointing unit-vector is always included, and pointing is a rotation matrix, rotation vector, euler vector, quaternion, etc. the yaw is a challenge for image recognition algorithms, which often assume a stable image orientation. for lost in space, there's no 'gravity' and no 'upwards and downwards' stabilizing the images.

 low-resolution 'lo-fi' star tracker images force recognition algorithms to be more robust and coarsely tuned. there's a bare minimum of information in the images, with only crude, global features, and little opportunity for over-fitting and fine-tuning. with randomly oriented lo-fi images of 'naked-eye visual' stars, recognition generally won't be too different than what the human brain does, using broad patterns and relationships between the individual stars.  

the 'foundational' nature of lo-fi star tracker image recognition is reflected in using the 28 x 28 pixel format of the [mnist database](https://en.wikipedia.org/wiki/MNIST_database). all incoming information is encoded in 28 x 28 pixels, and successful star identification algorithms are necessarily robust and fundamental.

here's a collection of earlier comments around the lost in space problem from this project.

...snip...

a vehicle in deep space accidentally tumbles out of control, loses power, and goes into a 'safety mode'. later, emergency systems regain power and stop the tumbling. the vehicle's star tracker snaps an image. what an onboard computer needs to do is match the stars in the image with entries in a star catalog. it's then straightforward to determine the vehicle's orientation in space.

it turns out that the essence of the problem doesn't focus on two dimensional images. instead, the focus is on three dimensional unit vectors. true, for unit vectors one of the three coordinates is redundant. given the other two, the unit constraint determines the third. but each star is essentially 'purely a direction' in the star tracker's reference frame. purely a pointing vector. the image encodes a set of unit vectors pointing out of an origin in various directions. with appropriate knowledge of the star tracker's characteristics, a set of three dimensional unit vectors are immediately extracted from the image.

this parallels the nature of star catalogs. star catalogs are collections of three dimensional unit vectors in an agreed on celestial reference frame. these days, the celestial frame is tied to the pointing directions to quasars, astronomical objects distant enough to be effectively 'fixed in space'. fixed landmarks as it were. star catalogs also provide names, brightnesses, etc, but here the core information is 'purely pointing direction'.

so the lost in space problem is about matching a set of unit vectors in a sensor reference frame with a set of unit vectors in a star catalog reference frame. the unit vectors can point towards any patch of directions 'on the sky'. it's really about the whole sphere of the night sky, that's one of the core elements here.  

...snip...

the answer was, no. there wasn’t an easy or obvious solution, and helping to figure out a practical method of identifying those particular stars on those particular plates was ultimately the real job. not that an undergrad had any chance of finding a real solution. but just becoming aware of and recognizing the magnitude of the problem was a huge step forward. how are stars recognized? humans could do it, but could an eighties computer system?

surprisingly, though, it soon became clear that there was still no publicly-available software for the lost-in-space star identification problem. apparently, each time star identification software had been developed, it’s been within classified or industry projects. a serious interest in star identification was probably tied to selling star trackers, and that’s become a fairly mature industry.

...snip...

cultural differences between aerospace and computer science quickly became apparent. basically, networks want to be about two dimensional images, while aerospace wants to be about physical three dimensional unit vectors. what happened in practice was that a kind of image 'api' grew up organically over the unit vector geometry. this happened over a period of a few months, and a curious sequence of coincidences took place.

low resolution makes the star identification problem more challenging and interesting. it forces use of global structures and patterns within an image, rather than localized features and heuristics. there’s simply less information available and more has to be done with less. it even suggests questions about how the human brain recognizes stars. for example, a typical high-resolution aerospace algorithm might focus on the exact separation between a pair of stars, along with the angle to a third star. that’s clearly not how the brain works. so, what's the brain in fact doing?

focusing on lo-fi mnist-like images led to a discovery. to recognize a particular star in an image, it's helpful to shift the star to the image-center and make its presence implicit. there’s no point in including it in the image, it's effectivly redundant. what’s significant is the relative-geometry of the other stars. the target star becomes the origin of the coordinate system, and if there’s another star nearby, as often happens in a low resolution image, there’s no confusion. in practice, the effects are even nicer since, in a way, there's a 'free' extra star, and there's also less need for coordinate transformations. 
