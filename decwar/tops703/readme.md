# simh specializations for pdp10-kl running top-10 7.03 and decwar

the foundation here is [open-simh](https://github.com/open-simh/simh), thanks to bob supnik and group.

on top of that, richard cornwell has built the [simh scripts and tape library](https://sky-visions.com/dec/tops10.shtml) used here. huge thanks to rich.

## minimal f10v6 system

    decwar-create-disk.ini - creates disk from tape
    decwar-boot-from-disk.ini - can install decwar from tape, uncomment or comment out the 'do decwar.do'.
