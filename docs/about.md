---
layout: page
title: about
---
this has been a hobby project for well over three decades. the code in the github repository grew over time, as well as the discussions that are now becoming part of this website. the code was used with python sphinx, and recently it became apparent that github jekyll could convert that into a static website.  

the 'lost in space' problem - [photos](https://photos.app.goo.gl/ifuTJUNsaRJK21E79){:target="_blank" rel="noopener"}
--------------------------

a vehicle in deep space accidentally tumbles out of control, loses power, and goes into a 'safety mode'. later, emergency systems regain power and stop the tumbling. the vehicle's star tracker snaps an image. what an onboard computer needs to do is match the stars in the image with entries in a star catalog. it's then straightforward to determine the vehicle's orientation in space.

it turns out that the essence of the problem doesn't focus on two dimensional images. instead, the focus is on three dimensional unit vectors. true, for unit vectors one of the three coordinates is redundant. given the other two, the unit constraint determines the third. but each star is essentially 'purely a direction' in the star tracker's reference frame. purely a pointing vector. the image encodes a set of unit vectors pointing out of an origin in various directions. with appropriate knowledge of the star tracker's characteristics, a set of three dimensional unit vectors are immediately extracted from the image.

this parallels the nature of star catalogs. star catalogs are collections of three dimensional unit vectors in an agreed on celestial reference frame. these days, the celestial frame is tied to the pointing directions to quasars, astronomical objects distant enough to be effectively 'fixed in space'. fixed landmarks as it were. star catalogs also provide names, brightnesses, etc, but here the core information is 'purely pointing direction'.

so the lost in space problem is about matching a set of unit vectors in a sensor reference frame with a set of unit vectors in a star catalog reference frame. the unit vectors can point towards any patch of directions 'on the sky'. it's really about the whole sphere of the night sky, that's one of the core elements here.  

for matching the two sets of unit vectors, the key characteristics are the angular separations. angular separations of unit vectors immediately connects with a core operation in numerical computing, the dot product, summing the products of unit vector components. and these angular separations concern the mutual relationships between pairs of unit vectors, so the focus becomes pairs of star vectors, and then triplets of pairs. it's a bit of linguistic abuse, but triplets of pairs can be thought of as 'triangles', and pairs can be thought of as 'sides'.

early history
------------------------------

key elements of lost in space are an automated star tracker mounted on a vehicle, images of the night sky, star catalogs for interpreting the images, and computing to bring them all together. the earliest appearance of all these elements seems to be with project febe in 1948, immediately after ww2. it seems likely this was both the earliest date that the sensing and computing were available, and that there was sufficient motivation to make the attempt. the motivations concern inertial guidance and navigation and immediately connect with a range of fascinating topics. to reduce distractions, the focus will be kept on the lost in space problem here, as close to star trackers and the night sky as possible. inertial sensors generally mean foreknowledge of what part of the sky is in view, which is the opposite of pure lost in space.

early systems, such as project febe, generally locked on to and tracked single stars. for true lost in space, an imaging star tracker with a ccd style pixelized sensor is needed, and those became available in the seventies and eighties. a related situation arose earlier though, namely, interpreting photographic plates from astronomical telescopes. here's an example that can be referred to as the 'asteroid problem'.

reflected sunlight from asteroids is dim because of their small size and distance, so large telescopes are needed to gather enough light. the resulting images cover a relatively small patch of sky and lots of faint stars. definitely a smaller and fainter patch of sky than normal for the naked eye. the question can arise, what's the star catalog entry for one of the faint stars in an asteroid image? for the lost in space problem, the field of view and star brightness are relatively large, limiting the number of potential stars to thousands. for the asteroid problem, the small field and brightness increases that number to tens of thousands or more. but at the same time, the pointing of the astronomical telescope is known, and the effective star catalog can be reduced to dim stars near the telescope's pointing vector.

1990 - [photos](https://photos.app.goo.gl/vKBxieTbwsbmshCg8){:target="_blank" rel="noopener"}
-------------------------------------------------------------

summer of ninety - university of texas at austin astronomy department - recently the hubble space telescope had finally reached orbit and the berlin wall had fallen - rent was less than two hundred a month, just a short walk north of campus - the eighty-three year old landlady and friend of lbj and john connally, mrs zively, lived directly overhead. mobile phones, notebook computers, and the web were all somewhere over the horizon - home internet was a dialup modem into a university access point. 

since the previous fall, work meant the hubble space telescope astrometry team - a group with members from the astronomy department, mcdonald observatory, the aerospace engineering department, the center for space research, and the european space agency and its hipparcos project - paul hemenway, an astronomer involved with all those organizations, was mentor and friend. life on the top three floors of rlm, the physics math astronomy tower, was special. high above the green treetops of austin, looking west to the hill country, it was a quiet and mellow time. a hippy vibe permeated the scene. wheatsville food coop, a few blocks away, was still in the seventies, which weren't so long ago after all.

hubble was designed for very exact and stable pointing to minimize motion smear in its images - three optical interferometers were mounted on robotic arms in the hubble’s focal plane to provide feedback to the pointing control system - these fine guidance sensors were a cutting-edge solution given seventies and eighties technology, with its uneasy mix of the analog and digital eras. exact calibrations were needed on-orbit to make the whole complex system work as intended.

asteroids were the calibration targets - mcdonald observatory and csr efforts for hubble fgs calibration became a component of the texas minor planet project. with group members in the astronomy and aerospace departments, asteroid orbit determinations were refined to the point where predicted positions and motions were effective as ground-truth references for comparison with hubble observations. 

the primary tools were a dec vax/vms cluster at csr, the main astronomy department vax running bsd unix, various early sun workstations, and the eighty-two inch telescope at mcdonald. it could image asteroids onto glass photographic plates, given enough time. a data general nova minicomputer and the suitcase sized cassegrain camera were used for the long exposures needed to gather enough light from these dim objects. 

this was another complex piece of custom-made analog hardware - for locking on to and tracking a guide star, it used an image dissector, a large photomultiplier tube attached to the camera’s side that looked a bit like an oil-filter. its circular field of view was divided into four quadrants - a guide star was manually positioned at the center and the closed-loop control system was activated - every second, with a loud mechanical knock, the system would bodily adjust the camera position and try to keep the guide star at the center - on a green phosphorescent screen, a fuzzy spot bounced about, now nearer the center of the cross hairs, now further out.

an observing night began a few hours before sunset. down in the telescope control room, circling the building beneath the telescope floor, the minicomputer and its control programs had to be started - outside on the catwalk at the base of the dome was a spectacular view of the shadows growing out across the high desert from the mountains - various obsolete computers and mysterious bits of hardware crowded the dark corners of the control room - the curving walls, low ceiling, red leds, and glowing crt terminals completed the brooding sci-fi ambience. 

a stack of white envelopes, each containing a glass photographic plate, waited on a desk - to prepare, a control room terminal with a command line program on the nova generated telescope pointing information for a list of asteroids. using this mini was probably the last serious contact with the large eight-inch floppy disks - they were a vanishing breed by the late 80s. after jotting down notes for the planned observations, the plates were moved up into the dome, where it was pitch black except for clouds of stars in the open slit - the telescope loomed overhead in the darkness.

caution was needed climbing up the stairs onto the circular telescope floor - it rose and descended in order to stay near the camera as the telescope moved - it was easy to take a step onto thin air in the dark, high above the dome floor. the massive base of the telescope and attached camera hung at eye level - sliding out the plate holder cover opened a rectangular frame of stars, with the silhouette of the telescope secondary mirror housing and its support struts high above. 

mcdonald maintenance staff had mounted the camera and connected power cables, but fine tuning was always needed, and the telescope itself had to be focused - this meant adjusting the position of the secondary mirror. a rocker switch on the telescope hand controller activated a motor to move the secondary inward or outward - the exact determination of focus was old-school, using a knife-edge. 

in the telescope’s focal plane, all of the light from a star converges through a single point. when a knife-edge cuts through that point, the light from the star is cut off instantly - if the knife-edge dims the star gradually, then the secondary mirror position needs to be adjusted. the point of instant cut off needed to be where the photographic plates were held by the camera, so a special metal frame mounting a straight knife-edge was fastened into the plate holder, for adjusting the secondary mirror position while watching the knife-edge. 

if there was a bit of spare time, the knife-edge frame could be replaced with a frame holding 'the eyepiece'. this was a glass lens about the size of a small flower vase and heavy enough to require both hands to lift. peering inside, the eye saw vivid red or green colors in nebula's - much like the popular photos.

once the telescope was ready, camera configuration came next. with the small field of view of the telescope - effectively a high magnification - asteroids moved significantly, relative to the background stars, over an interval of around ten minutes - each asteroid was a bit different, and various orbital characteristics had to be taken into account - the direction and rate of relative motion had already been computed by the minicomputer - now the camera body was rotated in its mounting, relative to the telescope, and programmed to move at the apparent rate of the target asteroid, so it would appear to be motionless. 

the rear-surface of the image dissector was a round crt screen divided by lines into four quadrants. light from a star, cascading down through the photomultiplier tube, formed a green glow on the screen. a guide-star was found near the asteroid and centered on the screen - with the tracking loop activated, the camera position was updated every few seconds with a mechanical click, keeping the star at the center of the screen. 

an asteroid exposure usually began with guide-star tracking - then the steady clicking of the tracking loop would go silent for a period of asteroid tracking, with asteroid-light building up a darker spot on the photograph plate - then the guide-star tracking would resume. the result was a dumbbell shape for stars, with two circular peaks connected by a trail - the asteroid was a trail with a circular peak at its midpoint. these peaks and trails became visible the next day when the plates were developed. each plate had many dumbbell shaped stellar trails - short or long, thick or thin - and at the center of the plate, a single ufo shaped asteroid-image.

reducing the glass plates to digital data, and then improving the asteroid orbital parameters followed over the next weeks and months - this all took place back in austin, where the center for space research and department of aerospace engineering became involved - their expertise in orbit determination played an important role in the hubble astrometry team. the space age was roughly thirty years old at the time, and members of its first generation led the center for space research - ray duncombe, byron tapley, and bob schutz. 

first, the plates had to be measured using a scanner and minicomputer in the scanning room, hidden behind a nondescript door in the astronomy department library on the fifteenth floor of robert lee moore hall - better known simply as rlm. many hours passed in the scanning room - it was a meditative kind of place, cool and dark, with a steady loud drone from electronics fans. the long back wall was covered with cabinets containing thousands of glass plates, including historic sets of survey plates from palomar and the european southern observatory, alongside many plates from mcdonald.

black plastic sheets from ceiling to floor formed made kind of cave of the back half of the room, and inside sat the pds microdensitometer. this was a machine for mechanically scanning photographic glass plates - an interesting time capsule of analog-era technology. light from a bulb was focused into a beam shooting downward through a mechanically driven stage with position encoders. a photometer below the stage measured the transmitted intensity while the stage moved in a raster pattern. sampling of the photometer and encoders was done by a very early, mini-fridge sized, rack-mounted sun workstation.

may or june of ninety was the first observing run at mcdonald - chat among the astronomers was about serious problems with hubble that were repeatedly making headline news - there was still lots of discussion of the high gain antennas, because news of the catastrophic error in the primary-mirror hadn’t yet leaked out - overhearing the veterans during those days at mcdonald was an early revelation about the realities of science and technology. the real world seemed a bit different than the popular science coverage.

it was an eight-hour drive to west texas from austin - three or four nights were occupied with making plates with the telescope - then came the drive back to austin. texas summer heat was just beginning to get intense, and after a sweltering walk over to rlm it was nice to settle into the cool darkness of the scanning room - that little apartment at mrs zively's house could be uncomfortably warm during the day, even with the air conditioning running full blast.

the plates were roughly the size and shape of writing paper - the glass was fairly thin and fragile - held up against a background light, the star and asteroid trails were small dark smudges. with the plate secured to the pds scanning stage, and looking across the plate’s surface, dull black trails of photographic emulsion were obvious on the surface of the glass, and the control software on the workstation had to be told which trails to scan. 

this meant moving the scanning beam about the plate, manually steering the stage and noting coordinates - at the top of the pds, roughly at eye level, was a circular glass screen showing a magnified image of the plate illuminated by the scanning beam - individual grains of photographic emulsion were visible, and when the beam was near a star trail it appeared as a fuzzy black worm. the stage was adjusted using two finely geared knobs, and the coordinates of the scanning beam were shown by two sets of red leds on the pds console - the corners of a rectangle about a star trail were the coordinates for a raster scan, and were entered in manually at the workstation keyboard.

the workstation was a tall rack standing in the back-right corner and mounting a mini-fridge sized early sun microsystems box - on a table beside the rack was an extremely heavy old crt monitor showing one of the first primitive unix graphical user interfaces, the sunview competitor of x windows - this machine already had the obsolete feel of an earlier era. the hardware seemed especially ponderous and heavy, as if made from scrap steel in an old-school factory - which is probably pretty close to the truth.

a scanning session meant creating a set of digitized raster files, one file for each trail scanned by the pds, archived on 9-track half-inch tape - a group of files, say thirty to fifty for a plate with a good exposure and lots of stars, was created in the filesystem of the workstation and then written to tape using its sibling above on the sixteenth floor, which had the tape drive.

the shift over the border from analog to digital took place in the seventies style electronics connecting the pds to the workstation. a few days after scanning those first plates, paul and ray duncombe discussed the next steps in wrw, the aerospace building. it was a short walk from rlm to wrw. overhead was the usual hard blue texas summer sky with little white clouds, and a blazing sun. sweat was running down just seconds after stepping outside the air conditioning.

during that short walk, lightning struck - exactly which stars were on those plates? how could those stars really, in practice, be identified, in order to determine the position of the asteroid? was there a program on the astronomy or aerospace computers to do that? 

the answer was, no. there wasn't an easy or obvious solution, and helping to figure out a practical method of identifying those particular stars on those particular plates was ultimately the real job. not that an undergrad had any chance of finding a real solution. but just becoming aware of and recognizing the magnitude of the problem was a huge step forward. how are stars recognized? humans could do it, but could an eighties computer system?

2003 - [photos](https://photos.app.goo.gl/ng8Nbxra2RYrbeWA7){:target="_blank" rel="noopener"}
-------------------------------------------------------------

thirteen years later, the work for the next eleven years was with bob schutz and the icesat group, directly within the center for space research. the focus was on, star trackers, modern descendents of maritime sextants for celestial navigation, and inertial sensors, often referred to simply as gyros. 

the problems, once again, concerned images containing a scattering of unknown stars. within aerospace, it’s a classic problem - the 'lost in space' problem. given an image of some stars, exactly which stars are they? 

aerospace has its own perspectives, culture, and tools - astronomers don’t generally think in terms of three-dimensional unit vectors, rotation matrices, quaternions, and vector-matrix notation. it was very quickly apparent that the concerns and methods in aerospace were more widely applicable and practical than those in astronomy, bringing together optimization, control, data fusion, high performance computing to solve practical real-world problems. 

within weeks of beginning, star identification was again one of the top concerns - and once again the first question was whether a practical solution was already available. pete shelus from the hubble astrometry days was an important member of the icesat group and pointed out useful directions. there was a strong sense of continuity and awareness that here was a problem that really needed addressing. the obvious differences now were that computing hardware was more powerful, and digital imaging had become standard. there was no longer an analog to digital divide to cross, everything was already in binary.

icesat’s control system usually made it straightforward to predict which stars each image contained - this wasn’t obvious or straightforward at first and it took effort to really understand the data coming from the spacecraft. there were four star imagers and three different different types of hardware onboard, all sampling at ten hertz or more. these were classic eighties star trackers and didn't provide star identifications. there was also higher-frequency angular-rate data from the inertial unit, and tracking data from the control system, so a pointing vector could usually be estimated for each star-image.

it was usually enough to check whether star-images with appropriate brightnesses were near their predicted positions. brightness information tends to muddy the star identification problem because it’s relatively difficult to measure and predict for a particular imager. images have better geometric information than brightness information. an astronomer interested in star brightness does photometry with dedicated sensors, not with imagers. 

an additional check on star recognition was that angles between observed star pairs matched predictions, and one of the first objectives was to model systematic errors in these angles from flight data. focusing on star pairs is a big step in the direction of looking at star triangles, and the core of project starid really formed at this time. at pete's suggestion, there was a bit of discussion with judit reese over in astronomy. and an early 'sourceforge' project and collaboration with undergrads happened around 2004 to 2006. this was essentially 'pre git' and sourceforge was using cvs and svn. git was just being created at that time specifically to replace cvs, svn, sourcesafe, etc.

it turned out there's a fascinating, though relatively small, literature on star identification and related topics. by the second world war, many large aircraft had a bubble window facing upward for a navigator to make stellar observations. after the war, computing and imaging automated the process. 

the cold war brought new motivations for the technology. people became uneasily aware of guidance systems via hollywood tales of spies and gyros. while most of the massive efforts went into integrated circuits and inertial guidance sensors, automated star tracking quietly matured in parallel. star trackers are critical for spacecraft, and are used on high altitude aircraft and missiles. the classical period was the sixties through the eighties, as digital systems really made automation practical. 

surprisingly, though, it soon became clear that there was still no publicly-available software for the lost-in-space star identification problem. apparently, each time star identification software had been developed, it’s been within classified or industry projects. a serious interest in star identification was probably tied to selling star trackers, and that’s become a fairly mature industry.

2016 - [photos](https://photos.app.goo.gl/z54G7X9dEop1e81y6){:target="_blank" rel="noopener"}
-------------------------------------------------------------

another thirteen years passed. excitement was growing again around neural networks in combination with recent hardware, especially at google, which had just open sourced tensorflow. this was definitely a curiosity, as the ai winter following the eighties was still fresh in mind. particularly as csr's offices were in the building built especially for [mcc in austin](https://en.wikipedia.org/wiki/Microelectronics_and_Computer_Technology_Corporation), which was said to have had the largest concentration of lisp machines in the world. 

for a number of reasons, it was clearly time to tackle the lost in space problem directly, using both geometric and network methods in parallel where possible. the concept was to start from scratch as a github open source project, integrating tensorflow from the beginning. this meant working in cpp eigen and python numpy.

the only external input was to be a list of star positions, and nasa’s skymap star catalog was an ideal source. skymap was created specifically for use with star trackers, so was important for icesat. there was even some collaboration where possible with skymap people. 

when hubble was launched, one of its early problems was bad guide stars. as part of the overall hubble recovery effort, nasa pushed skymap forward as an improved description of the sky, as seen by standard star trackers. one side effect of all that was that a small number of people came to be involved with star trackers and star catalogs. some of those people played a role in what happened with icesat.

skymap is simply a list of star positions, so how does one generate a star image? the core problem is searching for neighbors of an arbitrary point on a sphere. for example, given a list of points on earth, which of the points are near a particular latitude and longitude? the usual answers involve dividing the sphere up into tiles, transforming and subdividing, etc. even a 'square sky' is not unheard of. 

a more dynamic and flexible approach was published by daniele mortari. it’s related to lookup and hash tables, with some interesting quirks. it starts off by viewing stars as unit vectors with three coordinates, each between plus-one and minus-one. the idea is to search for stars within a small range around each coordinate. there are three thin rings on the sky, one around each coordinate-axis. the stars inside the small region where the rings intersect are 'near the target'. this is three independent searches for small ranges of values, followed by an intersection of the results.

each search is performed on a separate precomputed key-value table. the sorted keys are from plus-one to minus-one. values represent star labels. performance can be improved by fitting a curve to the sorted keys, then using it to calculate the bounding lower and upper indexes into the table. this creates something like a range search hash-table, with the fitted curve acting as a 'range hash function'.

cultural differences between aerospace and computer science quickly became apparent. basically, networks want to be about two dimensional images, while aerospace wants to be about physical three dimensional unit vectors. what happened in practice was that a kind of image 'api' grew up organically over the unit vector geometry. this happened over a period of a few months, and a curious sequence of coincidences took place.

standard nineties star tracker images were eight degrees by eight degrees - 28,000 arcseconds per side, roughly sixteen times the apparent diameter of the moon. the hello world problem in networks, mnist, was standardized in the late nineties using data files and images with 28 pixels per side. adapting those standards resulted in star images with thousand-arcsecond pixels. at first, actual mnist data files were simply overwritten with star images, then fed into standard network processors. gradually, additional advantages became apparent, beyond data file format compatibility. 

the effects go much deeper than merely nice rounding properties. they effectively mean low resolution, at the level of a toy camera or blurry mobile phone photo. by comparison, real star tracker images can involve sub-arcsecond resolutions. in the lingo of austin's nineties garage-rock punk scene, these mnist style star images are 'lo-fi'. usually a good thing - keeping it real.

low resolution makes the star identification problem more challenging and interesting. it forces use of global structures and patterns within an image, rather than localized features and heuristics. there’s simply less information available and more has to be done with less. it even suggests questions about how the human brain recognizes stars. for example, a typical high-resolution aerospace algorithm might focus on the exact separation between a pair of stars, along with the angle to a third star. that’s clearly not how the brain works. so, what's the brain in fact doing?

focusing on lo-fi mnist-like images led to a discovery. to recognize a particular star in an image, it's helpful to shift the star to the image-center and make its presence implicit. there’s no point in including it in the image, it's effectively redundant. what’s significant is the relative-geometry of the other stars. the target star becomes the origin of the coordinate system, and if there’s another star nearby, as often happens in a low resolution image, there’s no confusion. in practice, the effects are even nicer since, in a way, there's a 'free' extra star, and there's also less need for coordinate transformations. 

all the way back to ninety, it was clear that the shapes of triangles formed by a star field can be used to identify the stars - and that iterative and even recursive processes could be involved. but once triangles come in, they tend to multiply, which seems uncomfortable - where does it end? skipping ahead to the answer, enlightenment arrives with a simple restatement of the lost-in-space problem.

_start with a set of candidate star identities, then iteratively set aside those that can’t be correct, until only one remains._

it’s brute force, and deeper insights are likely possible, but the main thing is - it works.

between the star-level and triangle-level is the pair-level. in practice, it’s the fundamental structural unit. pair-level 'sides' make up the triangles. soon after code for mnist style star images came code for star pairs separated by less than eleven degrees on the sky. this was the fourth use of the key-value table described above, to represent nearly one million pairs as angles and member star identifiers. 

the initial concept was to focus on groups of four stars instead of just three. for a triangle of three stars, adding a fourth provides significantly more information - six edges instead of three, two of which are a shared pair. two sets of possible stars for the two triangles have to agree via the shared-pair. with low resolution, this isn't as useful it sounds - there are too many pairs that meet low resolution constraints. a low resolution shared-pair just doesnt provide enough unique information, it’s too ambiguous. in other words, at low resolution many of the skies triangles are similar. 

eventually, the concept of the shared-pair became the real focus. any pair of stars can be a shared-pair parent of many child-triangles. with the target star implicit in the center of an image containing ten other stars, there are ten shared-pairs that include the target star. each of these is the parent of nine child-triangles.

further reading
================

icesat
-----------------------------

2017, effect of sun shade performance on icesat-2 laser reference sensor alignment estimation, patel, smith, bae, schutz, aas advances in the astronautical sciences, [pdf](papers/2017%20aas.pdf)

2014, laser reference sensor alignment tracking and star observations, smith, bae, schutz, aiaa journal of spacecraft and rockets, [pdf](papers/2014%20jsr%20c.pdf)

2014, laser reference sensor alignment estimation using star observations, smith, bae, schutz, aiaa journal of spacecraft and rockets, [pdf](papers/2014%20jsr%20b.pdf)

2014, laser reference sensor alignment estimation using reference signal observations, smith, bae, schutz, aiaa journal of spacecraft and rockets, [pdf](papers/2014%20jsr%20a.pdf)

2013, nasa icesat 2 pointing knowledge, smith, bae, schutz, nasa technical report, [pdf](papers/2014%20nasa%20icesat%20b.pdf)

2012, nasa icesat 1 pointing knowledge, bae, smith, schutz, nasa technical report, [pdf](papers/2014%20nasa%20icesat%20a.pdf)

2011, improved star tracker instrument magnitude prediction from icesat flight telemetry, smith, fowell, bae, schutz, aas advances in the astronautical sciences, [pdf](papers/2011%20aas.pdf)

2010, biased star tracker measurements of forty-nine stars from flight data, smith, bae, schutz, aiaa journal of spacecraft and rockets, [pdf](papers/2010%20jsr.pdf)

2010, forty-nine biased star positions from icesat flight data, smith, bae, schutz, aas advances in the astronautical sciences, [pdf](papers/2010%20aas%20b.pdf)

2010, icesat attitude and pointing correction using the laser reference sensor, bae, smith, schutz, aas advances in the astronautical sciences, [pdf](papers/2010%20aas%20a.pdf)

2009, time tag issues in the star tracker and gyro data for icesat precision attitude determination, bae, ricklefs, smith, schutz, aas advances in the astronautical sciences, [pdf](papers/2009%20aas%20b.pdf)

2009, bad stars, fowell, smith, bae, schutz, aas advances in the astronautical sciences, [pdf](papers/2009%20aas%20a.pdf)

2008, precision orbit and attitude determination for icesat, schutz, bae, smith, sirota, aas advances in the astronautical sciences, [pdf](papers/2008%20aas.pdf)

star identification
-----------------------------

1977, star pattern recognition for real time attitude determination, junkins, [pdf](papers/1977%20junkins.pdf)

1993, pattern recognition of star constellations for spacecraft applications, liebe, ieee aerospace and electronic systems magazine, [pdf](papers/1993%20liebe.pdf)

1997, a grid algorithm for autonomous star identification, padgett, [pdf](papers/1997%20padgett.pdf)

1997, star identification using neural networks, lindblad, lindsey, [pdf](papers/1997%20lindblad.pdf)

1999, skymap specification, sande, ottenstein, nasa technical report, [pdf](papers/1999%20skymap%20a.pdf)

2004, the pyramid star identification technique, mortari, samaan, bruccoleri, junkins, advances in the astronautical sciences, [pdf](papers/2004%20mortari.pdf)

2006, non dimensional star identification for uncalibrated star cameras, samaan, [pdf](papers/2006%20samaan.pdf)

2008, an analytic approach to star identification reliability, kumari, mortari, [pdf](papers/2008%20kumari.pdf)

2009, a survey on star identification algorithms, spratling, mortari, [pdf](papers/2009%20spratling.pdf)

2012, an autonomous star identification algorithm based on the directed circularity pattern, xie, [pdf](papers/2012%20xie.pdf)

2012, star identification using delaunay triangulation and distributed neural networks, miri, shiri, [pdf](papers/2012%20miri.pdf)

2013, analysis of star identification algorithms due to uncompensated spatial distortion, bratt, [pdf](papers/2013%20bratt.pdf)

2014, a brightness referenced star identification algorithm for aps star trackers, zhang, [pdf](papers/2014%20zhang.pdf)

2014, stars a software application for the ebex autonomous daytime star cameras, chaoman, [pdf](papers/2014%20chapman.pdf)

2015, an autonomous star identification algorithm based on one dimensional vector pattern for star sensors, luo, [pdf](papers/2015%20luo.pdf)

spacecraft attitude
-----------------------------

2006, the quest for better attitudes, shuster, [pdf](papers/2006%20shuster.pdf)

texas minor planet project
-----------------------------

1986, the use of space telescope to tie the hipparcos reference frame to an extragalactic reference frame, hemenway, duncombe, astrometric techniques, [pdf](papers/1986%20hemenway.pdf)

1986, apparent close encounters of minor planets, pierce, whipple, the astronomical journal, [pdf](papers/1986%20pierce.pdf)

1987, current developments in the texas minor planet project, hemenway, whipple, duncombe, [pdf](papers/1987%20hemenway.pdf)
