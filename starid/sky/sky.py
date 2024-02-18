"""interactive model of the sky, based on a set of stars from the nasa skymap star catalog. the stars are defined by
a brightness cutoff - all stars brighter than the cutoff. with a cutoff of visual magnitude 6.5, this means slightly
more than all stars visible to human eyes - 8876 in total."""
from math import pi, cos, sin, sqrt, floor
import numpy as np
from starid.sky.skymap import Skymap
from starid.sky.geometry import FloatsIndexer, rotation_matrix, normalized
from starid.sky.image import Image

class Sky:
    """model the sky, based on the skymap object. the key input parameter is the star brightness threshold - with
    visual magnitude 6.5 the sky is about nine thousand stars, and that number grows exponentially as dimmer stars
    are included."""

    def __init__(self):
        self.stars = []
        self.axes = [FloatsIndexer(), FloatsIndexer(), FloatsIndexer()]
        self.max_ang = 1.4 * image_radius_radians
        pass

    def generate(self, pathskymap):
        """initialize the sky. first generates a skymap object and then picks out the information needed here,
        with some enrichment - in particular with three-dimensional vectors in the celestial reference frame."""
        skymap = Skymap(pathskymap)
        for ndx, rec in enumerate(skymap.records):
            star = Star()
            star.starndx = ndx
            star.skymap_number = rec.skymap_number
            star.mv = rec.mv1
            star.ra_degrees = 15.0 * (rec.rah + rec.ram / 60.0 + rec.ras / 3600.0)
            star.dec_degrees = rec.decsign * (rec.decd + rec.decm / 60.0 + rec.decs / 3600.0)
            ra = star.ra_degrees * pi / 180.0
            dec = star.dec_degrees * pi / 180.0
            star.x = cos(ra) * cos(dec)
            star.y = sin(ra) * cos(dec)
            star.z = sin(dec)
            self.axes[0].add_pair(star.x, ndx);
            self.axes[1].add_pair(star.y, ndx);
            self.axes[2].add_pair(star.z, ndx);
            self.stars.append(star)
        for axis in self.axes: axis.sort()
        return

    def image_of_target(self, starndx):
        """creates a standard image for the target star, ready for feeding into a star identifier. the
        format is 28 x 28 pixels - lo-fi, the way we like it. makes thing tougher on us. and also by no coincidence
        matching the classic mnist character recognition data set. the story behind that is a long one,
        discussed elsewhere in the project."""
        sts = self.stars
        target = sts[starndx]
        pointing = np.array([[target.x, target.y, target.z]]).T
        starndxs = self.stars_near_point(pointing, starndx)
        pvecs = np.asarray([[sts[n].x, sts[n].y, sts[n].z] for n in starndxs])
        info = [[sts[n].starndx, sts[n].skymap_number, sts[n].ra_degrees, sts[n].dec_degrees] for n in starndxs]
        attitude = rotation_matrix(pointing)
        pvecs = (attitude.T @ pvecs.T).T
        return Image(pvecs, info)

    def stars_near_point(self, pointing, targndx):
        """given a three-dimensional pointing vector in the celestial reference frame, return the
        identifiers for nearby stars. this is fundamental - we have to be able to call up the stars near a target on
        the sky. it's a rich problem we'll be discussing throughout the project documentation. here we break the
        three-dimensional search space down into three one-dimensional search spaces, and create a map or hash-index
        into each of those. in a sense - it's a three-dimensional hash map into the sky."""
        xring = self.stars_in_ring(pointing[0, 0], 0)
        yring = self.stars_in_ring(pointing[1, 0], 1)
        zring = self.stars_in_ring(pointing[2, 0], 2)
        starndxs = sorted(list(set(xring).intersection(set(yring)).intersection(set(zring))))
        starndxs = [x for x in starndxs if not x == targndx]  # target star is implicit
        return starndxs

    def stars_in_ring(self, p, n):
        """when we break the skies three-dimensional search space down into three one-dimensional search
        spaces, the one-dimensional spaces represent rings on the sky. we have three rings, and the stars we're
        interested in are in their intersection. this intersection of three rings is in some sense a
        three-dimensional hash map into the sky."""
        if p >= cos(self.max_ang):
            pmin = p * cos(self.max_ang) - sqrt(1 - (p * p)) * sin(self.max_ang)
            pmax = 1.0
        elif p <= -cos(self.max_ang):
            pmin = -1.0
            pmax = p * cos(self.max_ang) + sqrt(1 - (p * p)) * sin(self.max_ang)
        else:
            pmin = p * cos(self.max_ang) - sqrt(1 - (p * p)) * sin(self.max_ang)
            pmax = p * cos(self.max_ang) + sqrt(1 - (p * p)) * sin(self.max_ang)
        return self.axes[n].findndxs(pmin, pmax)

class Star:
    starndx = 0
    skymap_number = 0
    mv = 0.
    ra_degrees = 0.
    dec_degrees = 0.
    x = 0.
    y= 0.
    z = 0.
