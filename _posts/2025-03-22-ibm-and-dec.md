---
layout: post
title: "ibm7094 and dec10"
---

parallel simh fortran workflows for 7094 and 10 are taking shape and it's a good time to look at the two together. these are both big iron mainframe systems that were dominant and standard setting. they're both thirty-six bit and so interwoven with the tail-end of the history of punchcards. hollerith's paper cards transmuted into six bit bcd binary-coded decimal characters, aka 'six bit ascii'. six bit characters are ultimately the root explanation for the thirty-six bit mainframes. six divides evenly into thirty-six, not thirty-two.

the 7094 roughly associates with a period around 1960, the 10 with a period around 1970. more exactly, something like 1957 to 1969 for the 7094, and 1966 to 1983 for the 10. in any case, the overall feel and 'spirit' of the 7094 is firmly in 1957, rand/sac, big business territory. an altogether different feel and spirit surrounds the 10. the traditional 'batch' versus 'interactive' story is part of it, but a big part of that story is shaped by the physical hardware available in the two different time-frames, and that's becoming nicely apparent in the simh fortran workflows.

the tape drives of a 7094 are prominent in photos, maybe even iconic. five or six refrigerator-sized cabinets in a row with twin 'owl eye' tape reels staring from each. the reason is that a tape was effectively a 'file', one continuous stream of bcd characters read both backwards and forwards. the five or six tape drives are effectively the entire 'file system' of the 7094, just five or six files! 

by the time of the 10, a tape contained multiple files, like a disk, and the 'file system' had taken modern form, with six character names and suffixes such as .for, .txt, etc.
