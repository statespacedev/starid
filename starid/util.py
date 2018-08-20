import numpy as np
import matplotlib.pyplot as plt
import libstarid
ls = libstarid.libstarid()

def write_sky():
    ls.write_sky()

def read_sky(pathsky):
    ls.read_sky(pathsky)
    image_info = ls.image_info(3)
    image = np.zeros((28,28))
    for rowndx in range(len(image_info)):
        image[int(image_info[rowndx,0]), int(image_info[rowndx,1])] = 1.0
    plt.matshow(-1 * image, cmap='Greys', interpolation='nearest')
    plt.show()

if __name__ == "__main__":
    pass