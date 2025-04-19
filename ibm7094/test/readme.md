what's the minimum tape writing for algebra.map?

be able to write floats out as tape record/line.

one way to do this could be 'convert float to packed bcd word'. since it's now known how to write a packed bcd word as a tape record/line, this would work.

# test2a.map, writing to tape, begin exps with macros

why can't there be blank lines/cards?

why does the channel command ioct need to be after it is used?

why does macro def need to be before use?

# test2b.map, extend exps with macros

able to write alphameric literal to tape

# test2c.map, vfd pseudo op

discovery that vfd variable field definition pseudo op can directly create a packed bcd word. at this point, was still using two consecutive bcd words for two result values per record/line.  

# test2d.map, macro with arg for incoming vfd word

vfd variable field definition may be the way to convert float to packed bcd word. switch to one bcd word and result value per record/line.

    wrd1   vfd  h36/.98765
    wrd2   vfd  h36/.12345

is same as

    chr2   equ  9
    chr3   equ  8
    wrd1   vfd  6/27,6/chr2,6/chr3,6/7,6/6,6/5
    wrd2   vfd  6/27,6/1,6/2,6/3,6/4,6/5

combining equ and vfd pseudo ops is crucial. this should be the way to dynamically build the appropriate bcd word at runtime.
