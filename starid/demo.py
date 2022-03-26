"""handle calls to libstarid.cpp."""
import sys
import os
import numpy as np
import matplotlib.pyplot as plt
import math
import pprint
sys.path.append('/tmp/cmake-build-debug/libstarid')
sys.path.append('./cmake-build-debug/libstarid')


class Demo:
    """handle calls to libstarid.cpp."""

    def __init__(self):
        self.dirdata = '/tmp/data/'
        self.filecat = 'cat'
        self.filesky = 'sky'
        self.filestarpairs = 'starpairs'
        import libstarid
        self.api = libstarid.Api()
        if not os.path.exists(self.dirdata + self.filesky):
            self.api.write_sky(self.dirdata + self.filesky, self.dirdata + self.filecat)
        self.api.read_sky(self.dirdata + self.filesky)
        if not os.path.exists(self.dirdata + self.filestarpairs):
            self.api.write_starpairs(self.dirdata + self.filestarpairs)
        self.api.read_starpairs(self.dirdata + self.filestarpairs)

    def check_data(self):
        if not os.path.exists(self.dirdata + self.filesky): return False
        if not os.path.exists(self.dirdata + self.filestarpairs): return False
        return True

    def plot(self, starndx):
        """generate a standard lo-fi image for starndx, with the sky randomly rotated. this is an image for which we could perform star identification."""
        imgdict = self.api.image_generator(starndx)
        info = imgdict['info'] # use info to generate a 28 by 28 image pixel matrix
        image = np.zeros((28, 28))
        for rowndx in range(len(info)): image[int(info[rowndx, 0]), int(info[rowndx, 1])] = 1.0
        starlist = [] # info ready for writing nouns, verbs, and sentences
        for row in info:
            if row[0] == 0: continue
            starndx = int(row[2])
            x = row[1] - 13.5
            y = 13.5 - row[0]
            r = math.ceil(math.sqrt(x**2 + y**2) * 100.) / 100.
            starlist.append([starndx, int(row[0]), int(row[1]), x, y, r])
        starlist = sorted(starlist, key=lambda x: x[5])
        pprint.pprint(starlist)
        plt.matshow(-1 * image, cmap='Greys', interpolation='nearest')
        plt.show()

    def SETTLER(self, starndx):
        '''generate a star image for starndx and give it to SETTLER. the resulting id from the identifier should be the same as starndx.'''
        img = self.api.image_generator(starndx)
        id = self.api.SETTLER(img['pixels'])
        print('SETTLER, starndx %i, result %i' % (starndx, id))

    def NOMAD(self, starndx):
        '''generate a star image for starndx and give it to NOMAD. the resulting id from the identifier should be the same as starndx.'''
        img = self.api.image_generator(starndx)
        id = self.api.NOMAD(img['pixels'])
        print('NOMAD, starndx %i, result %i' % (starndx, id))


if __name__ == '__main__':
    starid = Demo()
    starid.SETTLER(starndx=3)
    starid.NOMAD(starndx=3)
    starid.plot(starndx=3)
    pass
