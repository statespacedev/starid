from starid.skymap import Skymap
from starid.star import Star

class Sky:
    """model the sky, based on the skymap object. the key input parameter is the star brightness threshold - with
    visual magnitude 6.5 the sky is about nine thousand stars, and that number grows exponentially as dimmer stars
    are included."""

    def __init__(self):
        self.stars = []
        pass

    def start(self, pathskymap):
        """initialize the sky. first generates a skymap object and then picks out the information needed here,
        with some enrichment - in particular with three-dimensional vectors in the celestial reference frame."""
        skymap = Skymap(pathskymap)
        for rec in skymap.records:
            star = Star(rec)
            self.stars.append(star)
        return
