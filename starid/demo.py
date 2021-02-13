import numpy as np
import matplotlib.pyplot as plt
import math, os, sys
sys.path.append('build/cmake/libstarid')
import libstarid

class Config():
    """Configuration including libstarid."""
    def __init__(self, args=None):
        self.cwd = os.getcwd() # expect */starid, not */starid/starid
        self.args = args
        self.dirsky = './data/'
        self.namecat = 'cat'
        self.namesky = 'sky'
        self.ls = libstarid.libstarid()
        if not os.path.exists(self.dirsky + self.namesky):
            self.ls.write_sky(self.dirsky + self.namesky, self.dirsky + self.namecat)
        self.ls.read_sky(self.dirsky + self.namesky)

def demo(args):
    """well ok"""
    conf = Config(args)
    img = Starimg(conf, targetndx=3)
    img.print_starlist()
    img.plot_image()

class Starimg:
    """Image coming from libstarid as info."""
    def __init__(self, conf, targetndx):
        self.conf = conf
        self.targetndx = targetndx
        imgdict = self.conf.ls.image_generator(targetndx)
        self.info = imgdict['info'] # use info to generate a 28 by 28 image pixel matrix
        self.image = np.zeros((28,28))
        for rowndx in range(len(self.info)):
            self.image[int(self.info[rowndx, 0]), int(self.info[rowndx, 1])] = 1.0
        self.starlist = [] # info ready for writing nouns, verbs, and sentences
        for row in self.info:
            if row[0] == 0: continue
            starndx = int(row[2])
            x = row[1] - 13.5
            y = 13.5 - row[0]
            r = math.ceil(math.sqrt(x**2 + y**2) * 100.) / 100.
            self.starlist.append([starndx, int(row[0]), int(row[1]), x, y, r]) # todo check that pixels are unique
        self.starlist = sorted(self.starlist, key=lambda x: x[5]) # sorted(self.starlist, key = lambda x: (x[3], x[0])

    def plot_image(self):
        plt.matshow(-1 * self.image, cmap='Greys', interpolation='nearest')
        plt.draw()
        plt.pause(2)
        plt.close()

    def print_starlist(self):
        import pprint
        pprint.pprint(self.starlist)

if __name__ == '__main__':
    pass