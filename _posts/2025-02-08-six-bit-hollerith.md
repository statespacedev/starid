---
layout: post
title: "six bit and hollerith"
---

[six bit character codes](https://en.wikipedia.org/wiki/Six-bit_character_code#Types_of_six-bit_codes) are a distinguishing characteristic of fifties and sixties computing. pdp-10, ibm 7094, and f66 in particular. six bit leads directly back via [bcd](https://en.wikipedia.org/wiki/BCD_(character_encoding)), [mag tape](https://en.wikipedia.org/wiki/IBM_7-track), and [punchcards](https://en.wikipedia.org/wiki/Punched_card) to [herman hollerith](https://en.wikipedia.org/wiki/Herman_Hollerith). for f66 code, an associated distinguishing characteristic is [hollerith constants](https://en.wikipedia.org/wiki/Hollerith_constant). if it's got seven or eight bits for more than sixty-four character symbols, or the 'character' data type, it's too new for projects simplex, inverse, and algebra. amusingly, it's still possible to encounter six bit in modern systems. maritime ais radio messages are a [good example](https://gpsd.gitlab.io/gpsd/AIVDM.html#_ais_payload_data_types). 

for project simplex, the objective is to use f66 on pdp10 and ibm 7094 to do a simplex solve using the original 1953 pfi product form of inverse. this means hollerith constants in the f66 code. and with the 7094 it should hopefully mean an f66 source deck on simh emulated punch cards and storing eta vectors on simh emulated mag tape, as described [here](https://statespace.dev/basis-inverse.html).

the essential fact of the era was punchcards and the large installed base of card punching and reading hardware. ibm bcd six bit was an adaption of punchcard hardware's hollerith codes. in a nutshell, punchcard hardware was the iron fact of reality, and drove the six bit character codes. this was memorialized in fortran up through f66 by the term 'hollerith constant'.

here's a way to think of the combination 'hollerith' and 'constant'. at the time, a fortran program was a deck of punchcards. period. hardware for punching and reading cards was what there was. a card represented a 'line' of the program. a certain card xyz could contain some 'constants', simple 'concrete facts'. these constants were made up of hollerith codes, which could be punched on a card. in some sense, a hollerith constant is a 'virtual punchcard'. card xyz can contain 'virtual cards' treated as 'concrete facts' by the fortran compiler.


