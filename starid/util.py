import numpy as np
import matplotlib.pyplot as plt
import libstarid
ls = libstarid.libstarid()

def write_sky(pathcat, pathsky):
    ls.write_sky(pathcat, pathsky)

def read_sky(pathsky):
    ls.read_sky(pathsky)
    images = ls.image_generator(3)
    image = np.zeros((28,28))
    for rowndx in range(len(images)):
        image[int(images[rowndx,0]), int(images[rowndx,1])] = 1.0
    plt.matshow(-1 * image, cmap='Greys', interpolation='nearest')
    plt.show()

if __name__ == "__main__":
    read_sky('../data/sky')
    pass