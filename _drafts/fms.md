---
layout: post
title: "fortran monitor system"
---
the fms fortran monitor system preceded and led to the ibsys operating system. both were tape based, while still being locked into the punchcard framework. a tape was in some sense a 'virtual' card deck, simply a newer faster way to represent a stream of cards, without the physical paper cards themselves. each line of the file written to tape is a card, and the complete file is a deck.

looking at the [old jobs archived at rich's site](https://sky-visions.com/ibm/ibsys_examples.shtml), many begin with the first lines/cards. here's the signature of the fms fortran monitor. 'execute fortran' means 'enter the fortran monitor'.

    $job           name1
    $execute       fortran

and here's the later and in some sense more capable ibsys sequence. this enters the 'ibsys ibjob processor', sets the job processing for a 'core memory storage map', and then enters the 'ibftc fortran compiler'. assembly and cobol are other options for the last card.

    $job           name2
    $execute       ibjob
    $ibjob         map
    $ibftc  deck1  list,sdd,nodeck

another difference is that in an fms tape/deck the fortran code main body has to end with the non standard 'call exit' instead of 'stop'. and there are special monitor command lines/cards beginning with '*'. it seems that the 'classic' fms was associated with fortran ii. it's not clear whether fortran iv was possible within fms or not. there's a later 'fortran iva monitor system' within the ibsys framework that may be the iv descendant of the classic ii fms.

for the ibsys sequence, the [ibftc](http://www.bitsavers.org/pdf/ibm/7090/C28-6390-3_v13_F4_Apr66.pdf) card is for the 'fortran iv compiler' component of the ibjob processor, which is a component of the ibsys operating system. this can serve as a baseline/reference fortran compiler. 


