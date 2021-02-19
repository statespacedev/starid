import os
dirin = './libstarid/'
dirout = './docs/libstarid_/'
files = ['api', 'sky', 'startriangles', 'starpairs']

for file in files:
    pathin = dirin + file + '.cpp'
    pathout = dirout + file + '.py'
    with open(pathin, 'rt') as fin, open(pathout, 'wt') as fout:
        for line in fin:
            if line[1:3] == '* ' and not line[3] == '*':
                outstr = line[3:]
                print(outstr)
                fout.write('%s' % (outstr))

