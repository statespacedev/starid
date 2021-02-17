class Skymap:
   '''bring the nasa skymap sky2000 v5r4 star catalog in. there are peculiarities to this catalog, and they should be reflected in its representation here. briefly, v5r4 was targeted at real world star tracker users - it tried to fuse results from multiple predecessor catalogs to provide useful information.'''
class Sky:
   '''model the sky, based on the skymap object. the key input parameter is the star brightness threshold - with visual magnitude 6.5 the sky is about 8000 stars, and that number grows exponentially as dimmer stars are included.'''
   def start(self, pathin):
      '''initializes the sky model. first generates a skymap model, and then picks out the information that is needed here - as well as enriching that info, in particular with three-dimensional vectors in the celestial reference frame.'''
