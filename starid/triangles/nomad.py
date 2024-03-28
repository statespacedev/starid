"""star recognition focused on a chain of triangles and basesides - side2 of each triangle is the baseside
of the following triangle. during feedback, these shared side2 -> baseside pairs are the path for information to flow
backwards - increasing the constraints on the initial triangle baseside and basestar. the name NOMAD relates to how
the chain of triangles wanders away from the target star and initial triangle."""
import numpy as np
from math import acos
from starid.triangles.star_triangle_side import Startriangleside
from starid.sky.geometry import arcseconds_to_radians
from starid.triangles.nomad_triangle import StartriangleNOMAD

class NOMAD:
    """recognize target star from triangles where the target star is the first basestar."""

    def __init__(self, starpairs):
        self.starpairs = starpairs

    def run(self, image):
        """recognize target star from the starvecs of image pixels."""
        starvecs, acands = image.starvecs(), []
        tri = [StartriangleNOMAD(starvecs, self.starpairs)]
        tri[-1].first()
        while not tri[0].stop():
            tri.append(StartriangleNOMAD(starvecs, self.starpairs))
            tri[-1].from_parent(tri[-2].side2, tri[-2].starb, tri[-2].starc)
            for ndx in range(len(tri) - 2, -1, -1):
                tri[ndx].chk2(tri[ndx + 1])
            print(len(tri[0].acands))
        result = tri[0].acands
        return result
