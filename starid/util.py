import numpy as np
import matplotlib.pyplot as plt
import libstarid
ls = libstarid.libstarid()

def write_sky(pathcat, pathsky):
    ls.write_sky(pathcat, pathsky)

def read_sky(pathsky):
    ls.read_sky(pathsky)
    imgdict = ls.image_generator(3)
    plt.matshow(-1 * imgdict['pixels'], cmap='Greys', interpolation='nearest')
    plt.show()

if __name__ == "__main__":
    read_sky('../data/sky')
    pass