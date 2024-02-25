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
        sva = np.array([[0., 0., 1.]]).T  # target star

        for ndxb, svb in enumerate(starvecs):  # abside
            abside = StarTriangleSide(acos(sva.T @ svb), self.starpairs)
            if acands: abside.update_acands(acands[-1])

            for ndxc, svc in enumerate(starvecs):  # abca triangle
                if ndxc == ndxb: continue
                ok, angsc = self.angsc(sva, svb, svc)
                if not ok: continue
                abca = SETTLERTriangle(svc, angsc[0], angsc[1], angsc[2], self.starpairs)
                abca.constrain()
                triangles = [abca]
                abside.update_abside(abca.side1)

                for ndxd, svd in enumerate(starvecs):  # abda triangle
                    if ndxd == ndxc or ndxd == ndxb: continue
                    ok, angsd = self.angsd(sva, svb, svc, svd, angsc)
                    if not ok: continue
                    abda = SETTLERTriangle(svd, angsd[0], angsd[4], angsd[3], self.starpairs)
                    abda.constrain2(triangles, self.starpairs)
                    triangles.append(abda)
                    abside.update_abside(abda.side1)

            acands.append(set(abside.stars.keys()))
            pass

    def angsd(self, sva, svb, svc, svd, angsc):
        """examine a candidate for star d before using it to form triangle abda. we want the angles from stars a, b,
        and c to be appreciable. the angles remain in angs_d for later use"""
        ok, angsd = True, [*angsc, acos(svd.T @ sva), acos(svd.T @ svb), acos(svd.T @ svc)]
        if any(ang < self.min_ang for ang in angsd): ok = False
        if abs(angsd[4] - angsd[3]) < self.min_ang: ok = False  # db-da
        if abs(angsd[4] - angsd[0]) < self.min_ang: ok = False  # db-ab
        if abs(angsd[4] - angsd[5]) < self.min_ang: ok = False  # db-dc
        return ok, angsd

    def angsc(self, sva, svb, svc):
        """examine a candidate for star c before using it to form triangle abca. we want the angles between stars a,
        b, and c to be appreciable. the angles remain in angs_c for later use."""
        ok, angsc = True, [acos(sva.T @ svb), acos(svb.T @ svc), acos(svc.T @ sva)]
        if any(ang < self.min_ang for ang in angsc): ok = False
        if abs(angsc[0] - angsc[1]) < self.min_ang: ok = False  # ab-bc
        if abs(angsc[0] - angsc[2]) < self.min_ang: ok = False  # ab-ca
        if abs(angsc[1] - angsc[2]) < self.min_ang: ok = False  # bc-ca
        return ok, angsc
