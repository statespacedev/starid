import numpy as np
import matplotlib.pyplot as plt
import libstarid
ls = libstarid.libstarid()

def read_sky(pathsky):
    ls.read_sky(pathsky)

def write_sky(pathsky, pathcat):
    ls.write_sky(pathsky, pathcat)

def read_starpairs(pathstarpairs):
    ls.read_starpairs(pathstarpairs)

def write_starpairs(pathstarpairs, pathcat):
    ls.write_starpairs(pathstarpairs, pathcat)

def test_sky(pathsky):
    read_sky(pathsky)
    imgdict = ls.image_generator(3)
    plt.matshow(-1 * imgdict['pixels'], cmap='Greys', interpolation='nearest')
    plt.show()

def test_starpairs(pathstarpairs):
    read_starpairs(pathstarpairs)

if __name__ == "__main__":
    #test_sky('../data/sky')
    test_starpairs('../data/starpairs')
    pass