"""NOMAD triangle. focus is on the basestar and baseside - nomad is about a chain of basesides,
each increasing the constraints on the preceding basestars. first constructor here is for the initial triangle
and has the target star as basestar. second constructor is for following triangles. each takes side2 from its
predecessor and uses that as its baseside. the chain of triangles is a train of baseides - side2 of each triangle
is the baseside of the following triangle. during feedback, these shared side2 -> baseside pairs are the path for
information to flow backwards, all the way backward from latest triangle to the initial triangle - increasing the
constraints on the initial triangle baseside and basestar."""
import numpy as np
from math import acos

import setuptools.command.install

from starid.triangles.star_triangle_side import StarTriangleSide

class NOMADTriangle:
    """NOMAD triangle. focus is on the basestar and baseside"""

    def __init__(self, starvecs, starpairs):
        self.starvecs, self.starpairs = starvecs, starpairs
        self.stara, self.starb, self.starc = 0, 0, 0
        self.sv1, self.sv2, self.sv3 = None, None, None
        self.side1, self.side2, self.side3 = None, None, None
        self.acands = set()
        self.tol = .003 # .0068
        pass

    def first(self):
        while self.starb == self.stara: self.starb += 1
        while self.starc == self.stara or self.starc == self.starb: self.starc += 1
        self.sv1, self.sv2, self.sv3 = np.array([0., 0., 1.]), self.starvecs[self.starb], self.starvecs[self.starc]
        self.side1 = StarTriangleSide(self.sv1, self.sv2, self.starpairs, angtol=self.tol)
        self.side2 = StarTriangleSide(self.sv2, self.sv3, self.starpairs, angtol=self.tol)
        self.side3 = StarTriangleSide(self.sv3, self.sv1, self.starpairs, angtol=self.tol)
        self.chk1()

    def from_previous(self, side2, starb, starc):
        self.stara, self.starb, self.starc = starb, starc, starc
        while self.starc == self.stara or self.starc == self.starb: self.starc += 1
        self.sv1, self.sv2, self.sv3 = self.starvecs[self.stara], self.starvecs[self.starb], self.starvecs[self.starc]
        self.side1 = side2
        self.side2 = StarTriangleSide(self.sv2, self.sv3, self.starpairs, angtol=self.tol)
        self.side3 = StarTriangleSide(self.sv3, self.sv1, self.starpairs, angtol=self.tol)
        self.chk1()

    def chk1(self):
        """in each of the three sides, there's a pairhalf1 -> pairhalf -> 0 or 1 concept. 0 is the default and means
        drop this pair. here we will mark pairs to keep by setting them to 1, all others will be dropped."""
        ok1, ok2, ok3 = set(), set(), set()
        for a1 in self.side1.stars:
            if a1 not in self.side3.stars: continue
            for b1 in self.side1.stars[a1]:
                if b1 not in self.side2.stars: continue
                for c2 in self.side2.stars[b1]:
                    if c2 not in self.side3.stars[a1]: continue
                    ok1.update([a1, b1])
                    ok2.update([b1, c2])
                    ok3.update([c2, a1])
        self.side1.update_side(ok1)
        self.side2.update_side(ok2)
        self.side3.update_side(ok3)

    def chk2(self, other):
        adside = StarTriangleSide(self.sv1, other.sv3, self.starpairs)
        ok1, ok2, ok3 = set(), set(), set()
        for a1 in self.side1.stars:
            if a1 not in self.side3.stars: continue
            for b1 in self.side1.stars[a1]:
                if b1 not in self.side2.stars: continue
                for c2 in self.side2.stars[b1]:
                    if c2 not in self.side3.stars[a1]: continue
                    if c2 not in adside.stars: continue
                    if a1 not in adside.stars or c2 not in other.side2.stars: continue
                    if len(set.intersection(adside.stars[a1], other.side2.stars[c2])) == 0: continue
                    ok1.update([a1, b1])
                    ok2.update([b1, c2])
                    ok3.update([c2, a1])
        self.side1.update_side(ok1)
        self.side2.update_side(ok2)
        self.side3.update_side(ok3)

    def stop(self):
        """stopping condition. true if basestars and basesides have been constrained to the point where only one
        possible basestar remains."""
        self.acands = set.intersection(set(self.side1.stars.keys()), set(self.side3.stars.keys()))
        if len(self.acands) > 1: return False
        else: return True