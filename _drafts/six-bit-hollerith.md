---
layout: post
title: "six bit and hollerith"
---

[six bit character codes](https://en.wikipedia.org/wiki/Six-bit_character_code#Types_of_six-bit_codes) are a distinguishing characteristic of fifties and sixties computing. pdp-10, ibm 7094, and f66 in particular. if it's got seven or eight bits, for more than 64 character symbols, it's too new for project simplex, project inverse, and project algebra. 

amusingly, it's still possible to encounter six bit in modern systems. maritime ais radio messages are a [good example](https://gpsd.gitlab.io/gpsd/AIVDM.html#_ais_payload_data_types). 

six bit leads directly back via [bcd](https://en.wikipedia.org/wiki/BCD_(character_encoding)), [mag tape](https://en.wikipedia.org/wiki/IBM_7-track), and [punchcards](https://en.wikipedia.org/wiki/Punched_card) to [herman hollerith, 1860-1929](https://en.wikipedia.org/wiki/Herman_Hollerith). for f66 code, an associated distinguishing characteristic is [hollerith constants](https://en.wikipedia.org/wiki/Hollerith_constant).

for project simplex, the objective is to use f66 on pdp10 and ibm 7094 to do a simplex solve using the original 1953 pfi product form of inverse. this means hollerith constants in the f66 code. and with the 7094 it should hopefully mean an f66 source deck on simh emulated punch cards and storing eta vectors on simh emulated mag tape, as described [here](https://statespace.dev/basis-inverse.html).
