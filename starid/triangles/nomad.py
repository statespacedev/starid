"""star recognition focused on a chain of triangles and basesides - side2 of each triangle is the baseside
of the following triangle. during feedback, these shared side2 -> baseside pairs are the path for information to flow
backwards - increasing the constraints on the initial triangle baseside and basestar. the name NOMAD relates to how
the chain of triangles wanders away from the target star and initial triangle."""
import numpy as np
from math import acos
from starid.triangles.star_triangle_side import StarTriangleSide
from starid.sky.geometry import arcseconds_to_radians
from starid.triangles.nomad_triangle import NOMADTriangle

class NOMAD:
    """recognize target star from triangles where the target star is the first basestar."""

    def __init__(self, starpairs):
        self.starpairs = starpairs
        self.min_ang = 3000. * arcseconds_to_radians
        self.tri = []
        self.maxtriangles = 90

    def run(self, image):
        """recognize target star from the starvecs of image pixels."""
        starvecs, acands = image.starvecs(), []
        self.tri.append(NOMADTriangle(starvecs, self.starpairs))
        self.tri[-1].first()
        while not self.tri[0].stop():
            self.tri.append(NOMADTriangle(starvecs, self.starpairs))
            self.tri[-1].from_parent(self.tri[-2].side2, self.tri[-2].starb, self.tri[-2].starc)
            for ndx in range(len(self.tri) - 2, -1, -1):
                self.tri[ndx].chk2(self.tri[ndx + 1])
            print(len(self.tri[0].acands))
        result = self.tri[0].acands
        return result
