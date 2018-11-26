import numpy as np
import matplotlib.pyplot as plt
import libstarid
ls = libstarid.libstarid()

def test_sky(pathsky):
    read_sky(pathsky)
    imgdict = ls.image_generator(3)
    plt.matshow(-1 * imgdict['pixels'], cmap='Greys', interpolation='nearest')
    plt.show()

def read_sky(pathsky):
    ls.read_sky(pathsky)

def write_sky(pathsky, pathcat):
    ls.write_sky(pathsky, pathcat)

def read_starpairs(pathstarpairs):
    ls.read_starpairs(pathstarpairs)

def write_starpairs(pathstarpairs):
    ls.write_starpairs(pathstarpairs)

def startriangles(starndx=3):
    read_sky('../data/sky')
    read_starpairs('../data/pairs')
    imgdict = ls.image_generator(starndx)
    id = ls.startriangles(imgdict['pixels'])
    print(id)

def test_starpairs(pathstarpairs):
    read_sky('../data/sky')
    write_starpairs('../data/starpairs')
    #read_starpairs(pathstarpairs)

if __name__ == "__main__":
    #startriangles(2)
    test_sky('../data/sky')
    #test_starpairs('../data/starpairs')
    pass