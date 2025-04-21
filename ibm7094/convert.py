#!/usr/bin/python
from definitions import bcd2oct, oct2bin

lines = [x.strip() for x in open('output', 'rt') if len(x.strip()) == 6]
for bcd in lines:
    octal, bits = '', ''
    for chr1 in bcd:
        a = bcd2oct[chr1]
        octal += a
        for chr2 in a:
            b = oct2bin[chr2]
            bits += b
    sign = -1 if int(bits[0]) > 0 else 1
    exp1 = [int(bits[x+1])*2**(7-x) for x in range(8)]
    exp = sum(exp1) - 128
    pass



