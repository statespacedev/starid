"""SETTLER triangle. acts as the triangles abca and abda within the star triangle identifier inner loops. there are
three triangle sides representing three star pairs, each with an angular separation. each side is acted by a star
triangle side object. star recognition focused on triangles that contain the target star - star a is always the
target star, star b is a neighbor star, and an abside is a star pair and triangle side with the target as the first
member of the pair. in the inner loops, additional stars c and d are involved. first an abca triangle is formed. this
constrains the abside. then for an abca triangle, a sequence of abda triangles are formed, further constraining the
abside. when we reach an abda that eliminates all but one star pair possibility for the abside, we've recognized the
target star. until that happens, we continue picking new absides, with new abca triangles, with new abda triangles.
the name SETTLER comes from the idea that we never move away the target star, we're settling around it."""
from math import acos
import numpy as np
from starid.triangles.star_triangle_side import Startriangleside

class StartriangleSETTLER:
    """acts as the triangles abca and abda within the star triangle identifier inner loops. there are three triangle
    sides representing three star pairs, each with an angular separation."""

    def __init__(self, sv2, sv3, starpairs, abside):
        self.tol = .003 # .0068
        sv1 = np.array([0., 0., 1.])
        self.side1 = Startriangleside(sv1, sv2, starpairs, angtol=self.tol)
        self.side2 = Startriangleside(sv2, sv3, starpairs, angtol=self.tol)
        self.side3 = Startriangleside(sv3, sv1, starpairs, angtol=self.tol)
        self.vecstar3 = sv3
        self.side1.stars = abside.stars
        self.abside = abside
        self.starpairs = starpairs

    def chk1(self):
        """test candidate star pairs for the sides of an abca triangle. we first look 'backwards' in side3,
        from star a to star c, and ask if star a is possible. then we look 'forwards' in side1 and side2 ask if star
        b is possible. last we look 'forwards' at side2 and side3 and ask if star c is possible."""
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
        self.abside.update_abside(self.side1)

    def chk2(self, triangles):
        """test candidate star pairs for the sides of an abda triangle. the added info and constraining possible here
        with abda was the big discovery that first got settler really going. you can see it here around the set
        intersection. the new question we're asking is essentially 'is the abda's star d even possible?' comparing
        with some prior triangle, we've got a side cd connecting their 'third stars', stars c and d, and we've also
        got the third side of the prior triangle, which we look at 'backwards', from a to c rather than 'forwards'
        from c to a. we're forcing there to be overlap/intersection for 'star c' - star c has to be possible in both
        cd and ac, this then implies 'ok, star d is possible'"""
        for triangle in triangles:
            cdside = Startriangleside(self.vecstar3, triangle.vecstar3, self.starpairs, angtol=self.tol)
            ok1, ok2, ok3 = set(), set(), set()
            for a1 in self.side1.stars:
                if a1 not in self.side3.stars: continue
                for b1 in self.side1.stars[a1]:
                    if b1 not in self.side2.stars: continue
                    for c2 in self.side2.stars[b1]:
                        if c2 not in self.side3.stars[a1]: continue
                        if c2 not in cdside.stars: continue
                        if a1 not in triangle.side3.stars: continue
                        if not set.intersection(cdside.stars[c2], triangle.side3.stars[a1]): continue
                        ok1.update([a1, b1])
                        ok2.update([b1, c2])
                        ok3.update([c2, a1])
            self.side1.update_side(ok1)
            self.side2.update_side(ok2)
            self.side3.update_side(ok3)
        self.abside.update_abside(self.side1)
