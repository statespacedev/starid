import sys, os
import numpy as np
import matplotlib.pyplot as plt
import math, pprint
sys.path.append('build/cmake/libstarid')
import libstarid

class PyLibstarid():
    """handles calls to libstarid.cpp"""
    def __init__(self):
        self.dirsky = './data/'
        self.namecat = 'cat'
        self.namesky = 'sky'
        self.api = libstarid.Libstarid()
        if not os.path.exists(self.dirsky + self.namesky):
            self.api.write_sky(self.dirsky + self.namesky, self.dirsky + self.namecat)
        self.api.read_sky(self.dirsky + self.namesky)

    def plot(self, targetndx):
        """plots a sky image for a target star"""
        imgdict = self.api.image_generator(targetndx)
        info = imgdict['info'] # use info to generate a 28 by 28 image pixel matrix
        image = np.zeros((28,28))
        for rowndx in range(len(info)):
            image[int(info[rowndx, 0]), int(info[rowndx, 1])] = 1.0
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
        plt.draw()
        plt.pause(2)
        plt.close()


