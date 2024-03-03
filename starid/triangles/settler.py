"""the target star ia always star a. star b is a neighbor star, and an abside is a star pair and
triangle side with the target as the first member of the pair. in the inner loops, additional stars c and d are
involved. first an abca triangle is formed. this constrains the abside. then for an abca triangle, a sequence of
abda triangles are formed, further constraining the abside. when we reach an abda that eliminates all but one
star pair possibility for the abside, we've recognized the target star. the name SETTLER comes from the idea that
we never move away the target star, we're settling around it."""
import numpy as np
from math import acos
from starid.triangles.star_triangle_side import StarTriangleSide
from starid.sky.geometry import arcseconds_to_radians
from starid.triangles.settler_triangle import SETTLERTriangle

class SETTLER:
    """recognize target star from triangles where the target star is always star a."""

    def __init__(self, starpairs):
        self.starpairs = starpairs
        self.min_ang = 3000. * arcseconds_to_radians

    def run(self, image):
        """recognize target star from the starvecs of image pixels."""
        starvecs, acands = image.starvecs(), []
        sva = np.array([0., 0., 1.])  # target star

        for ndxb, svb in enumerate(starvecs):  # abside
            abside, bypass = StarTriangleSide(sva, svb, self.starpairs), False
            if acands: abside.update_acands(acands[-1])

            for ndxc, svc in enumerate(starvecs):  # abca triangle
                if bypass or ndxc == ndxb: continue
                angsc = [acos(sva @ svb), acos(svb @ svc), acos(svc @ sva)]
                if any(ang < self.min_ang for ang in angsc): continue
                if abs(angsc[0] - angsc[1]) < self.min_ang: continue  # ab-bc
                if abs(angsc[0] - angsc[2]) < self.min_ang: continue  # ab-ca
                if abs(angsc[1] - angsc[2]) < self.min_ang: continue  # bc-ca
                abca = SETTLERTriangle(sva, svb, svc, self.starpairs)
                abca.side1.stars = abside.stars
                abca.chks1()
                triangles = [abca]
                abside.update_abside(abca.side1)

                for ndxd, svd in enumerate(starvecs):  # abda triangle
                    if bypass or ndxd == ndxc or ndxd == ndxb: continue
                    # angsd = [*angsc, acos(svd @ sva), acos(svd @ svb), acos(svd @ svc)]
                    abda = SETTLERTriangle(sva, svb, svd, self.starpairs)
                    abda.side1.stars = abside.stars
                    abda.chks2(triangles, self.starpairs)
                    triangles.append(abda)
                    abside.update_abside(abda.side1)
                    if len(abside.starcnt) > 3 and len(set(abside.starcnt[-3:])) == 1: bypass = True

            acands.append(set(abside.stars.keys()))
            if not len(acands[-1]) > 1: break
        result = acands[-1]
        return result

