import numpy as np
import matplotlib.pyplot as plt
import math, os

class Starimgs:
    def __init__(self, conf):
        self.conf = conf
        self.starndx = None # starndx of target star
        self.info = None # use info to generate a 28 by 28 image pixel matrix
        self.image = np.zeros((28,28))
        self.starlist = [] # info ready for writing nouns, verbs, and sentences
        self.lang = None

    def test(self):
        pathcat = self.conf.dirsky + self.conf.namecat
        pathsky = self.conf.dirsky + self.conf.namesky
        if not os.path.exists(pathsky):
            self.conf.ls.write_sky(pathsky, pathcat)
        self.conf.ls.read_sky(pathsky)
        imgdict = self.conf.ls.image_generator(3)
        import matplotlib.pyplot as plt
        plt.matshow(-1 * imgdict['pixels'], cmap='Greys', interpolation='nearest')
        plt.show()

    def addinfo(self, starndx, info):
        self.starndx = starndx # starndx of target star
        self.info = info # use info to generate a 28 by 28 image pixel matrix
        self.image = np.zeros((28,28))
        for rowndx in range(len(self.info)):
            self.image[int(self.info[rowndx, 0]), int(self.info[rowndx, 1])] = 1.0

    def genlang(self):
        for row in self.info:
            starndx = int(row[2])
            x = row[1] - 13.5
            y = 13.5 - row[0]
            r = math.ceil(math.sqrt(x**2 + y**2) * 100.) / 100.
            self.starlist.append([starndx, int(row[0]), int(row[1]), x, y, r]) # todo check that pixels are unique
        self.starlist = sorted(self.starlist, key=lambda x: x[5]) # sorted(self.starlist, key = lambda x: (x[3], x[0])
        if len(self.starlist) < 6: # too few stars
            self.lang = None
            return
        from starlanguages import Lang1
        self.lang = Lang1(self.starlist, self.starndx)

    def plot_image(self):
        plt.matshow(-1 * self.image, cmap='Greys', interpolation='nearest')
        plt.show()

    def print_starlist(self):
        import pprint
        pprint.pprint(self.starlist)

if __name__ == '__main__':
    pass