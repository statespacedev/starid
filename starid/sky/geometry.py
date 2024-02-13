"""geometric ops for working with three dimensional star unit vectors."""

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
