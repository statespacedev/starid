"""act as one of the three triangle sides within a parent star triangle object. here stars is a representation of
candidate star pairs that could belong to the side. ultimately - when we've recognized the target star, all but one
candidate star pair is eliminated. each side has a 'forward' direction inherent in its starpairs object."""
import math
from math import sqrt, acos
from starid.sky.geometry import arcseconds_to_radians

class Startriangleside:

    def __init__(self, sv1, sv2, starpairs, angtol=None):
        self.sv1, self.sv2, self.ang = sv1, sv2, acos(sv1 @ sv2)
        self.angtol = .003 # 2. * sqrt(500. * 500. + 500. * 500.) * arcseconds_to_radians
        if angtol: self.angtol = angtol
        self.stars = starpairs.pairs_for_angle(self.ang, self.angtol)
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
            if acand in self.stars:
                tmp2 = set.intersection(side.stars[acand], self.stars[acand])
                if not tmp2: continue
                tmp[acand] = tmp2
        self.stars = tmp
        self.starcnt.append(len(self.stars))
        self.paircnt.append(self.count_pairs())

    def update_acands(self, acands):
        """this is an abside and we're given a 'new info' set of a stars. shrink our a star possibilities."""
        drops = [k for k in self.stars if k not in acands]
        for k in drops: self.stars.pop(k, None)
        self.starcnt.append(len(self.stars))
        self.paircnt.append(self.count_pairs())
        return
