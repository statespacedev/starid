---
layout: post
title: "ibm7094 and dec10"
---

parallel simh fortran workflows for the 7094 and the 10 are taking shape and it's a good time to step back and look at them side-by-side. these are both big iron mainframe systems that were dominant and standard setting. they're both thirty-six bit, and so interwoven with the tail-end of the history of punchcards. hollerith's paper cards transmuted into six bit bcd binary-coded decimal characters, aka 'six bit ascii'. six bit characters are ultimately the root explanation for the thirty-six bit mainframes. six divides evenly into thirty-six, not thirty-two.

the 7094 roughly associates with a period around 1960, the 10 with a period around 1970. more exactly, something like 1957 to 1969 for the 7094, and 1966 to 1983 for the 10. in any case, the overall feel and 'spirit' of the 7094 is firmly 1957, rand/sac, industrial. an altogether different feel and spirit surrounds the 10. the traditional 'batch' versus 'interactive' divide is part of it. a big part of that story is shaped by the physical hardware available in the two different time-frames. that's becoming readily apparent in the two simh fortran workflows.

the tape drives of a 7094 are prominent in photos. maybe even iconic. ten or more refrigerator-sized cabinets in a row with twin 'owl eyes' tape reels staring from each. the root explanation here, is that a tape was a 'file'. one continuous stream of bcd characters read both backwards and forwards. ten tape drives are effectively the entire 'file system' of a 7094. ten files! 

by the time of the 10, a tape contained multiple files and the concept of a 'file system' had taken modern form, with six character file names and file suffixes such as .for, .txt, etc. 

this is an aspect of the change in 'feel'. the concept of a 'file system' had appeared, and was tied to the spread of disk based near-line storage. with the 7094 around 1960, drum memory was probably most common after tape. there was no need for a conceptual file system. files were concrete physical tapes. any system of files would simply be a pile of tapes or a group of operating tape drives. the 7094 would boot ibsys from one tape.

by the 10 and 1970, disks were not only standard, it was even normal and expected that a 10 would boot tops-10 from disk. tape had become secondary and was on its way to becoming a 'backup' for disks. disks contained files, structured in an overall 'file system', and so tapes now did as well.
