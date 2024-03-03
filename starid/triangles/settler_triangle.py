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
from starid.triangles.star_triangle_side import StarTriangleSide

class SETTLERTriangle:
    """acts as the triangles abca and abda within the star triangle identifier inner loops. there are three triangle
    sides representing three star pairs, each with an angular separation."""

    def __init__(self, sv1, sv2, sv3, starpairs):
        self.side1 = StarTriangleSide(sv1, sv2, starpairs)
        self.side2 = StarTriangleSide(sv2, sv3, starpairs)
        self.side3 = StarTriangleSide(sv3, sv1, starpairs)
        self.vecstar3 = sv3
        pass

    def chks1(self):
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

    def chks2(self, triangles, starpairs):
        """test candidate star pairs for the sides of an abda triangle. the added info and constraining possible here
        with abda was the big discovery that first got settler really going. you can see it here around the set
        intersection. the new question we're asking is essentially 'is the abda's star d even possible?' comparing
        with some prior triangle, we've got a side cd connecting their 'third stars', stars c and d, and we've also
        got the third side of the prior triangle, which we look at 'backwards', from a to c rather than 'forwards'
        from c to a. we're forcing there to be overlap/intersection for 'star c' - star c has to be possible in both
        cd and ac, this then implies 'ok, star d is possible'"""
        for triangle in triangles:
            cdside = StarTriangleSide(self.vecstar3, triangle.vecstar3, starpairs)
            ok1, ok2, ok3 = set(), set(), set()
            for a1 in self.side1.stars:
                if a1 not in self.side3.stars: continue
                for b1 in self.side1.stars[a1]:
                    if b1 not in self.side2.stars: continue
                    for c2 in self.side2.stars[b1]:
                        if c2 not in self.side3.stars[a1]: continue
                        if c2 not in cdside.stars: continue
                        if a1 not in triangle.side3.stars: continue
                        if len(set.intersection(cdside.stars[c2], triangle.side3.stars[a1])) == 0: continue
                        ok1.update([a1, b1])
                        ok2.update([b1, c2])
                        ok3.update([c2, a1])
            self.side1.update_side(ok1)
            self.side2.update_side(ok2)
            self.side3.update_side(ok3)
