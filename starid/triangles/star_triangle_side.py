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
        self.starcnt = [len(self.stars)]
        self.paircnt = [self.count_pairs()]
        pass

    def count_pairs(self):
        count = 0
        for k, v in self.stars.items(): count += len(v)
        return count

    def update_side(self, ok):
        drops = set()
        for star in self.stars:
            if star not in ok:
                drops.add(star)
            else:
                self.stars[star] = set.intersection(self.stars[star], ok)
                if len(self.stars[star]) == 0: drops.add(star)
        for star in drops: self.stars.pop(star, None)
        self.starcnt.append(len(self.stars))
        self.paircnt.append(self.count_pairs())

    def update_abside(self, side):
        tmp = dict()
        for acand in side.stars.keys():
            if acand in self.stars: tmp[acand] = set.intersection(side.stars[acand], self.stars[acand])
        self.stars = tmp
        self.starcnt.append(len(self.stars))
        self.paircnt.append(self.count_pairs())

    def update_acands(self, acands):
        tmp = dict()
        for acand in acands:
            if acand in self.stars: tmp[acand] = self.stars[acand]
        self.stars = tmp
        self.starcnt.append(len(self.stars))
        self.paircnt.append(self.count_pairs())
        return
