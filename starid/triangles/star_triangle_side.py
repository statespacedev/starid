"""act as one of the three triangle sides within a parent star triangle object. here stars is a representation of
candidate star pairs that could belong to the side. ultimately - when we've recognized the target star, all but one
candidate star pair is eliminated. each side has a 'forward' direction inherent in its starpairs object."""
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
        """the 'stars' here are star pairs, with a star1 key and set of stars2 pair mates. sum up all the stars2,
        across all the star1 possibilities."""
        count = 0
        for star1, stars2 in self.stars.items(): count += len(stars2)
        return count

    def update_side(self, ok):
        """here we're told 'ok, here's a set of stars that are possible for this side'. we want to drop all the stars
        that aren't in this 'ok set'."""
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
        """this is an abside and we're given a 'new info' abside. shrink our 'a star candidates' based on the
        new info."""
        tmp = dict()
        for acand in side.stars.keys():
            if acand in self.stars: tmp[acand] = set.intersection(side.stars[acand], self.stars[acand])
        self.stars = tmp
        self.starcnt.append(len(self.stars))
        self.paircnt.append(self.count_pairs())

    def update_acands(self, acands):
        """this is an abside and we're given a 'new info' set of a stars. shrink our a star possibilities."""
        tmp = dict()
        for acand in acands:
            if acand in self.stars: tmp[acand] = self.stars[acand]
        self.stars = tmp
        self.starcnt.append(len(self.stars))
        self.paircnt.append(self.count_pairs())
        return
