"""geometric ops for working with three dimensional star unit vectors."""
import numpy as np


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
    rm = np.eye(3)
    icrfz = np.asarray([[0., 0., 1.]]).T
    bodyx = normalized(np.cross(bodyz, icrfz, axisa=0, axisb=0).T, axis=0)
    bodyy = normalized(np.cross(bodyz, bodyx, axisa=0, axisb=0).T, axis=0)
    return np.hstack((bodyx, bodyy, normalized(bodyz, axis=0)))

def normalized(a, axis=-1, order=2):
    """standard unit vector. vector length/magnitude is one. L2 norm in math jargon."""
    l2 = np.atleast_1d(np.linalg.norm(a, order, axis))
    l2[l2==0] = 1
    return a / np.expand_dims(l2, axis)
