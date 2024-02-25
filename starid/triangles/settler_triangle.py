"""SETTLER triangle. acts as the triangles abca and abda within the star triangle identifier inner loops. their are
three triangle sides - representing three star pairs, each with an angular separation. each side is acted by a star
triangle side object. star recognition focused on triangles that contain the target star - star a is always the
target star, star b is a neighbor star, and an abside is a star pair and triangle side with the target as the first
member of the pair. in the inner loops, additional stars c and d are involved. first an abca triangle is formed. this
constrains the abside. then for an abca triangle, a sequence of abda triangles are formed, further constraining the
abside. when we reach an abda that eliminates all but one star pair possibility for the abside, we've recognized the
target star. until that happens, we continue picking new absides, with new abca triangles, with new abda triangles.
the name SETTLER comes from the idea that we never move away the target star, we're settling around it."""
from math import acos
from starid.triangles.star_triangle_side import StarTriangleSide

class SETTLERTriangle:
    """acts as the triangles abca and abda within the star triangle identifier inner loops. their are three triangle
    sides - representing three star pairs, each with an angular separation."""

    def __init__(self, sv, ang1, ang2, ang3, starpairs):
        self.side1 = StarTriangleSide(ang1, starpairs)
        self.side2 = StarTriangleSide(ang2, starpairs)
        self.side3 = StarTriangleSide(ang3, starpairs)
        self.vecstar3 = sv
        pass

    def constrain(self):
        """test candidate star pairs for the sides of an abca triangle."""
        ok1, ok2, ok3 = set(), set(), set()
        for star1side1 in self.side1.stars:
            if star1side1 not in self.side3.stars: continue
            for star2side1 in self.side1.stars[star1side1]:
                if star2side1 not in self.side2.stars: continue
                for star3side2 in self.side2.stars[star2side1]:
                    if star3side2 not in self.side3.stars[star1side1]: continue
                    ok1.update([star1side1, star2side1])
                    ok2.update([star2side1, star3side2])
                    ok3.update([star3side2, star1side1])
        self.side1.update_side(ok1)
        self.side2.update_side(ok2)
        self.side3.update_side(ok3)
        return

    def constrain2(self, triangles, starpairs):
        """test candidate star pairs for the sides of an abda triangle."""
        for triangle in triangles:
            cdang = acos(self.vecstar3.T @ triangle.vecstar3)
            cdside = StarTriangleSide(cdang, starpairs)
            ok1, ok2, ok3 = set(), set(), set()
            for star1side1 in self.side1.stars:
                if star1side1 not in self.side3.stars: continue
                for star2side1 in self.side1.stars[star1side1]:
                    if star2side1 not in self.side2.stars: continue
                    for star3side2 in self.side2.stars[star2side1]:
                        if star3side2 not in self.side3.stars[star1side1]: continue
                        if star3side2 not in cdside.stars: continue
                        if star1side1 not in triangle.side3.stars: continue
                        pairscdc = cdside.stars[star3side2]
                        pairsacc = triangle.side3.stars[star1side1]
                        if len(set.intersection(pairscdc, pairsacc)) == 0: continue
                        ok1.update([star1side1, star2side1])
                        ok2.update([star2side1, star3side2])
                        ok3.update([star3side2, star1side1])
            self.side1.update_side(ok1)
            self.side2.update_side(ok2)
            self.side3.update_side(ok3)
        return
