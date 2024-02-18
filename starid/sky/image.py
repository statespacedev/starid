"""standard image for a target star, ready for feeding into a star identifier. the format is 28 x 28 pixels -
matching the classic mnist character recognition data set. the target star is always implicit here, it's always at
the center of the image and doesn't need to be explicitly mentioned. all of the starvecs and image pixels are for the
target's near neighbors within the square image field of view."""
from math import pi, cos, sin, sqrt, floor
import numpy as np
import matplotlib.pyplot as plt
import random
from starid.sky.geometry import image_radius_radians, image_radius_unit_vector_plane, image_pixel_unit_vector_plane

class Image:

    def __init__(self, starvecs, info, doyaw=True):
        self.info, self.pixels, self.img = [], set(), np.zeros((28, 28))
        yaw = random.uniform(0., 2 * pi) if doyaw else 0.
        for ndx, pvec in enumerate(starvecs):
            x = cos(yaw) * pvec[0] - sin(yaw) * pvec[1]
            y = sin(yaw) * pvec[0] + cos(yaw) * pvec[1]
            axi = x + image_radius_unit_vector_plane
            axj = -y + image_radius_unit_vector_plane
            axindx = floor(axi / image_pixel_unit_vector_plane)
            axjndx = floor(axj / image_pixel_unit_vector_plane)
            if axjndx < 0 or axjndx > 27: continue
            if axindx < 0 or axindx > 27: continue
            self.pixels.add((axjndx, axindx))
            self.img[axjndx, axindx] = 1.0
            self.info.append([*info[ndx], axjndx, axindx])

    def plot(self):
        """generate a standard lo-fi image for starndx, with the sky randomly rotated. this is an image for which we
        could perform star identification."""
        plt.matshow(-1 * self.img, cmap='Greys', interpolation='nearest')
        plt.show()

    def starvecs(self):
        """convert image pixels to star pointing vectors / unit vectors in the 'camera frame'. these arent the 'real'
        hi-res starvecs we had when first creating the image. they're lo-res 'approximations' and have effectively lost
        or fuzzed out info during the transforms to and back from 28 x 28 pixels. several 'real' starvecs can end up as
        a single pixel."""
        starvecs = []
        for axjndx in range(28):
            for axindx in range(28):
                if self.img[axjndx, axindx] > 0:
                    x = image_pixel_unit_vector_plane * (-13.5 + float(axindx))
                    y = image_pixel_unit_vector_plane * (13.5 - float(axjndx))
                    starvecs.append([x, y, sqrt(1 - x**2 - y**2)])
        return np.asarray(starvecs)
