import numpy as np
import matplotlib.pyplot as plt
import libstarid
ls = libstarid.libstarid()

def write_sky(pathsky, pathcat):
    ls.write_sky(pathsky, pathcat)

def write_starpairs(pathstarpairs, pathcat):
    ls.write_starpairs(pathstarpairs, pathcat)

def read_sky(pathsky):
    ls.read_sky(pathsky)
    imgdict = ls.image_generator(3)
    plt.matshow(-1 * imgdict['pixels'], cmap='Greys', interpolation='nearest')
    plt.show()

if __name__ == "__main__":
    #read_sky('../data/sky')
    write_starpairs('../data/starpairs', '../data/sky')
    pass