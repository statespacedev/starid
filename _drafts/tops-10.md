---
layout: post
title: "tops-10"
---

on thursday june 6 2024 an article appeared on a news website about the [pidp-10](https://obsolescence.dev/pidp10.html). by saturday june 8 the simh hardware side of that was [running here local](https://photos.app.goo.gl/7BEa6RberDfcsWLp7). the efforts over the years to keep all of this alive had been followed all along via usenet, slashdot, and rss news feeds. the pidp-10 news just pushed a button. the timing of all this is interesting, as on april 1 had moved to a new website and the third post was about the [pdp-10](https://statespace.dev/pdp-10.html). 

for the first two or three weeks of june, the focus was on exploring its and simh. playing adventure for the first time in decades. first time running spacewar on a type 340 display. first time using emacs rather than sos or teco. hello world lisp program compiled and run. for background on why all of this is exciting in itself, can recommend firstly the [newquist ai book](https://a.co/d/0fizN5g7) and secondly the [levy book](https://a.co/d/0eyKbrFf). also the [arpanet book](https://a.co/d/0baPMYaQ). and, as always, [soul of a new machine](https://a.co/d/0baPMYaQ).

by the third and fourth weeks, attention shifted to tops-10. this post is simply a walk-through of why, and what it's all about.

first of all, the concept is to do some numerical code in assembly. and naturally, fortran as well. to make this concrete, to reproduce some of what feynman is discussing towards the end of [this chapter here](https://www.feynmanlectures.caltech.edu/I_22.html). purely for the pleasure of ideas. for actually doing this, the [gorin book](https://archive.org/details/introductiontodecsystem10assemblylangpgmgjul85text) seems to be a reasonable starting point. it's tops-10, macro, ddt based. and for fortran, there's [this from 69](https://bitsavers.org/pdf/dec/pdp10/TOPS10/DEC-10-AFC0-D_PDP-10_FORTRAN_IV_Programming_Manual_Aug69.pdf) and [this from 77](https://bitsavers.org/pdf/dec/pdp10/TOPS10/AA-0944E-TB_FORTRAN-10_Programmers_Reference_Ver_5_Jan77.pdf). 

and then, secondly and lastly. [decwar](http://hsnewman.freeshell.org/decwar.htm). official utaustin [catalog record](https://repositories.lib.utexas.edu/items/e0e0aa09-2dc3-49c5-a919-19210d7916d3/full). again, fortran and assembly.

while working on the previous paragraph, some questions were answered by the [wikipedia page](https://en.wikipedia.org/wiki/Decwar#Other_versions). especially note the following.
    
    Later the CIS (CompuServe Information Service) version of Decwar was released to the public by The University of Texas. This version was highly modified to run on CIS's version of TOPS-10. Merlyn Cousins (Drforbin) later reverse engineered the source back to a form which would run on standard TOPS-10 and simh. This code is archived at the Briscoe Center for American History.[8] The source is also available on GitHub.[9])

note - while creating this post, stumbled on this highly recommendable [article from 88](https://www.columbia.edu/cu/computinghistory/dec20.html). fun read. laughing out loud.
