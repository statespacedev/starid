---
layout: post
title: "life before terminals"
---

the pre terminal era reached its apogee with the ibm7094 around 1960. this was the golden age of tape. by the dec10 and 1970, interactive visual terminals were normal and expected. similar can be said of disks and file systems. tape played the roles that would soon come to be associated with terminals and disks. 

this all soon becomes clear when actually coding on the 7094. the 7094 manual discusses tape insistently. there's not a single mention of interactive terminals, because they didn't yet exist. the closest thing at the time was the sage console and its offspring the pdp-1 console, and they were still far from evolved into what could be associated with a modern terminal.

so what's coding with the 7094 and tape actually like? it's about using the 7094 data channels to move thirty-six bit words between core memory and sequential records on tape. a standard situation is that each word is composed of six six-bit bcd characters, 'six-bit ascii' roughly speaking, and that's assumed true for this discussion. 

a central concept here is that the core memory is shared by separate processes / processors. core memory is valuable so shared, not divided, between the cpu and tape drives. the data channel is a way for them to communicate with each other. the cpu executes ops at core memory location xyz that use the data channel to tell a tape drive to look in core memory location abc. the tape drive begins executing the ops it finds at location abc. meanwhile, the cpu has gone onward and is executing its next ops over at xyz.

the ops the tape drive finds at location abc are roughly 'grab the word at location ijk and write it on tape', or 'read a word from tape and put it at location ijk'. because core memory location ijk is always involved, thirty-six bit words are central. they're the structural foundation of the core memory, and therefore of the tape and tape drive.

the challenge is to have the proper bits in the word at core memory location ijk. the overall feel is relatively low level. bit shifting, masking, and packing become important. as usual for thirty-six bit systems, the bits are normally represented in octal notation. twelve octal values per word, six pairs of octals for bcd characters. the system monitor / os does provide important software ops to help, on top of the builtin hardware ops. these are termed pseudo ops in 7094 ibsys, muuo's in dec10 tops10. the ibsys pseudo ops are oriented around bit packing, while the tops10 muuo's cover character and line interactions with terminals. this represents an important advantage of fortran. the same fortran code for writing and reading should work on both 7094/ibsys and dec10/tops10, while the underlying lower level ops are completely different. 
