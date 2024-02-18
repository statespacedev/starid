"""act as one of the three triangle sides within a parent star triangle object. here stars is a representation of
candidate star pairs that could belong to the side. ultimately - when we've recognized the target star, all but one
candidate star pair is eliminated."""
import math
from math import sqrt
from starid.sky.geometry import arcseconds_to_radians

class StarTriangleSide:

    def __init__(self, ang, starpairs):
        angtol = 2. * sqrt(500. * 500. + 500. * 500.) * arcseconds_to_radians
        self.stars = starpairs.pairs_for_angle(ang, angtol)
        self.log_star_count = [len(self.stars)]
        self.log_pair_count = [self.count_pairs()]
        pass

    def count_pairs(self):
        """how many candidate pairs remain in this side"""
        count = 0
        for k, v in self.stars.items(): count += len(v)
        return count
