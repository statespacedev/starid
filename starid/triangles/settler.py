"""the target star ia always star a. star b is a neighbor star, and an abside is a star pair and
triangle side with the target as the first member of the pair. in the inner loops, additional stars c and d are
involved. first an abca triangle is formed. this constrains the abside. then for an abca triangle, a sequence of
abda triangles are formed, further constraining the abside. when we reach an abda that eliminates all but one
star pair possibility for the abside, we've recognized the target star. the name SETTLER comes from the idea that
we never move away the target star, we're settling around it."""
import numpy as np
from math import acos
from starid.triangles.star_triangle_side import Startriangleside
from starid.sky.geometry import arcseconds_to_radians
from starid.triangles.settler_triangle import StartriangleSETTLER

class SETTLER:
    """recognize target star from triangles where the target star is always star a."""

    def __init__(self, starpairs):
        self.starpairs = starpairs
        self.acands = []

    def run(self, image):
        """recognize target star from the starvecs of image pixels."""
        starvecs = image.starvecs()
        for b, svb in enumerate(starvecs):  # abside
            abside, bypass = Startriangleside(np.array([0., 0., 1.]), svb, self.starpairs), False
            if self.acands: abside.update_acands(self.acands[-1])
            for c, svc in enumerate(starvecs):  # abca triangle
                if bypass or c == b: continue
                tri = [StartriangleSETTLER(svb, svc, self.starpairs, abside)]
                tri[0].chk1()
                for d, svd in enumerate(starvecs):  # abda triangle
                    if bypass or d == c or d == b: continue
                    tmp = StartriangleSETTLER(svb, svd, self.starpairs, abside)
                    tmp.chk2(tri)
                    tri.append(tmp)
                    if abside.starcnt[-1] == abside.starcnt[-2]: bypass = True
            self.acands.append(set(abside.stars.keys()))
            if len(self.acands[-1]) <= 1: break
        result = self.acands[-1]
        return result

