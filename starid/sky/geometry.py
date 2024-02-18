"""geometric ops for working with three dimensional star unit vectors."""
import numpy as np
from math import pi

image_radius_radians = 0.0698131700797732  # four degrees in radians
star_pair_angle_limit = 0.197461463918150  # 2*sqrt(2)*image_radius_radians
image_radius_unit_vector_plane = 0.0697564737441253  # sin(4*pi/180)
image_pixel_unit_vector_plane = 0.00498260526743752  # sin(4*pi/180)/14
star_brightness_limit = 6.5  # star visual magnitude
arcseconds_to_radians = pi / 648000.0

class FloatsIndexer:
    """used for the x, y, z floats forming rings on the sky around the x, y, z axes. we want to give a min and max on
    an axis and get back the index values of stars in the ring, between min and max on the axis."""

    def __init__(self):
        self.float_ndx = []

    def add_pair(self, newfloat, newndx):
        """add a float value on an axis, and the index of the star."""
        self.float_ndx.append((newfloat, newndx))

    def sort(self):
        """sort based on the float values along an axis, so we can grab all stars between some min and max."""
        self.float_ndx.sort()

    def findndxs(self, lofloat, hifloat):
        """get all the (float, starndx) pairs between lofloat and hifloat. we want those starndxs."""
        starndxs = [x[1] for x in self.float_ndx if lofloat <= x[0] <= hifloat]
        return sorted(starndxs)

def rotation_matrix(bodyz):
    """standard three dimensional rotation matrix representing the baseline attitude of our 'camera's body' in space.
    the z axis is along the camera's optical axis and 'yaw' is around this axis. for the baseline attitude,
    yaw is zero. to randomize the two dimensional sky images we create, we'll later apply random yaws."""
    icrfz = np.asarray([[0., 0., 1.]]).T
    bodyx = normalized(np.cross(bodyz, icrfz, axisa=0, axisb=0).T)
    bodyy = normalized(np.cross(bodyz, bodyx, axisa=0, axisb=0).T)
    return np.hstack((bodyx, bodyy, normalized(bodyz)))

def normalized(a):
    """standard unit vector. vector length/magnitude is one. L2 norm."""
    b = a / np.linalg.norm(a)
    return b
