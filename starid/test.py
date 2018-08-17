import numpy as np
import matplotlib.pyplot as plt
import libstarid
ls = libstarid.libstarid()

def image_info_to_image(image_info):
    image = np.zeros((28,28))
    for rowndx in range(len(image_info)):
        image[int(image_info[rowndx,0]), int(image_info[rowndx,1])] = 1.0
    return image

def test():
    ls.load_sky()
    image_info = ls.image_info(3)
    image = image_info_to_image(image_info)
    plt.matshow(-1 * image, cmap='Greys', interpolation='nearest')
    plt.show()

def main():
    pass

if __name__ == "__main__":
    main()