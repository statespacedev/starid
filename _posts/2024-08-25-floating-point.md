---
layout: post
title: "floating point"
---

around 1950, fixed point binary representations were used for numbers. there was no floating point, and there was a strong current of thought that this was as it should be. if numbers were kept properly scaled, fixed point was all that was needed and floating point was an unnecessary compromise. von neumann was a champion of this view. what ended up actually happening is a curious bit of history involving the mit instrumentation laboratory, navigation, astronomy, and engineering.

a standard aerospace textbook by [richard battin](https://en.wikipedia.org/wiki/Richard_Battin) begins with a detailed description of how he and [hal laning](https://en.wikipedia.org/wiki/J._Halcombe_Laning) developed early missile guidance systems at the instrumentation laboratory in the fifties, particularly [q guidance](https://en.wikipedia.org/wiki/Q-guidance).
it turns out that at the same time hal laning was the lead programmer who brought floating point to the whirlwind computer in 1952. as part of that, he developed what may have been the first real compiler and high level language, called 'george' from 'let george do it'. here's a comment from [john backus](https://en.wikipedia.org/wiki/John_Backus).

_laning was the first person to conceive of and implement what we would now call a true compiler_

at first glance, george code is clearly a primitive form of fortran, and was immediately put to practical use in 1953 on 'a set of six nonlinear differential equations describing the lead-pursuit dynamics of an air-to-air fire-control problem'. this was for the sage continental air defense system, for which whirlwind was the brain.

at the same time, backus was working on bringing floating point to the ibm 701 'defense calculator'. he did this via an interpreter and virtual machine called 'speedcoding'. the floating point routines soon became hardware instructions in the follow-on ibm 704. backus is up-front that the speedcoding system involved compromises. 'george' was a high level language compiling to machine code, 'speedcoding' was not. within a few years, fortran brought these two strands together and floating point numerical computing was established as a pragmatic real world fact. all in some sense outside the academic computer science mainstream, where things were quickly heading towards cobol, lisp, 'words' rather than numbers, and the first wave of 'ai'.
