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

    def update_side(self, ok):
        """there's a pairhalf1 -> pairhalf -> 0 or 1 concept. 0 is the default and means drop this particular pair.
        here we drop all pairs that have not been set to 1, and reset all that remain to 0."""
        empty = set()
        for star in self.stars:
            self.stars[star] = set.intersection(self.stars[star], ok)
            if len(self.stars[star]) == 0: empty.add(star)
        for star in empty: self.stars.pop(star, None)
        self.log_star_count.append(len(self.stars))
        self.log_pair_count.append(self.count_pairs())

    def update_abside(self, side):
        tmp = dict()
        for acand in side.stars.keys():
            if acand in self.stars: tmp[acand] = side.stars[acand]
        self.stars = tmp
        self.log_star_count.append(len(self.stars))
        self.log_pair_count.append(self.count_pairs())

    def update_acands(self, acands):
        tmp = dict()
        for acand in acands:
            if acand in self.stars: tmp[acand] = self.stars[acand]
        self.stars = tmp
        self.log_star_count.append(len(self.stars))
        self.log_pair_count.append(self.count_pairs())
        return
