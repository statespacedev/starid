"""view the sky as triangles of stars. for the target star, it's a member of a set of triangles - eliminate candidate
ids based on the geometry of these triangles. this is an iterative process and the inner loop is comparing triangle
geometries. the overall speed depends on this inner loop, so the focus is on making it as efficient as possible.

identify the target of a star image, using the triangles formed by neighboring stars within the image. the
fundemental particles are actually pairs of stars - in a sense individual stars don't exist here, what exists are
pairs of stars, acting as sides of triangles - so a key object handed to the identifier in its constructor is a
starpairs object, containing all of the relevant pairs. when possible, the starpairs object is loaded from a
starpairs file, rather than generated at run-time."""
import numpy as np
from math import acos
from starid.sky.geometry import FloatsIndexer
from starid.sky.geometry import star_pair_angle_limit

class Starpairs:
    """foundation for star triangles - a star pair is a triangle-side - it's the fundamental particle of a triangle
    view of the sky. every triangle is made of three starpairs. in a starpair object, for each star, the starpairs
    with each of its near neighbors are represented, pre-computed and ready for use. computing this object is
    relatively heavy and we want to do it once, in advance, and then reuse it from there. so write it to a starpairs
    file and read in the starpairs object from that whenever possible, rather than generating from scratch."""

    def __init__(self):
        self.angndxs = FloatsIndexer()
        self.starpairs = []
        self.starpairsndxs = dict()

    def generate(self, sky):
        """create a starpairs object from scratch. this can written to disk and read from there in the future to
        bypass these computations."""
        pairndx = 0
        for star in sky.stars:
            starndxs = list(set(sky.stars_near_point(np.array([[star.x, star.y, star.z]]).T) + [star.starndx]))
            for starndx1 in starndxs:
                for starndx2 in starndxs:
                    if starndx1 == starndx2: continue
                    key = self.pair_labeler(starndx1, starndx2)
                    if key in self.starpairsndxs: continue
                    angle = acos((sky.stars[starndx1].x * sky.stars[starndx2].x) +
                                (sky.stars[starndx1].y * sky.stars[starndx2].y) +
                                (sky.stars[starndx1].z * sky.stars[starndx2].z))
                    if abs(angle) > star_pair_angle_limit: continue
                    self.starpairs.append((angle, starndx1, starndx2))
                    self.starpairsndxs[key] = pairndx
                    self.angndxs.add_pair(angle, pairndx)
                    pairndx += 1

    def pairs_for_angle(self, angle, tol_radius):
        """for an angle, what are the candidate star pairs? creates the representation of stars used in star
        triangles. there, a star is a collection of associations with its near neighbors - its essential feature is
        its membership in pairs and triangle sides. what we do here is look at each star in turn, asking the question
        - what pairings do we care about for the star triangle representation of the sky we're going to use? the
        tuning parameters representing the answer to that question are the angle between pair members and a measure
        of tolerance or sensitivity."""
        stars, ang1, ang2, epsilon = dict(), angle - tol_radius, angle + tol_radius, 1.
        if ang1 <= 0.: ang1 = 0.
        if ang2 <= epsilon * tol_radius: ang2 = epsilon * tol_radius
        if ang1 >= star_pair_angle_limit - epsilon * tol_radius: ang1 = star_pair_angle_limit - epsilon * tol_radius
        if ang2 >= star_pair_angle_limit: ang2 = star_pair_angle_limit
        pairndxs = self.angndxs.findndxs(ang1, ang2)
        for pairndx in pairndxs:
            star1, star2 = self.starpairs[pairndx][1], self.starpairs[pairndx][2]
            if star1 not in stars: stars[star1] = set()
            if star2 not in stars: stars[star2] = set()
            stars[star1].add(star2)
            stars[star2].add(star1)
        return stars

    @staticmethod
    def pair_labeler(catndx1, catndx2):
        """returns a unique string for the pair, consisting of the catalog ids for the member stars - a useful
        identifier for the pair"""
        if catndx1 > catndx2: return str(catndx2) + str(catndx1)
        else: return str(catndx1) + str(catndx2)
