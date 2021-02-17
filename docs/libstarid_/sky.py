class Skymap:
   '''bring the nasa skymap sky2000 v5r4 star catalog in. there are peculiarities to this catalog, and they should be reflected in its representation here. briefly, v5r4 was targeted at real world star tracker users - it tried to fuse results from multiple predecessor catalogs to provide useful information.'''
class Sky:
   '''model the sky, based on the skymap object. the key input parameter is the star brightness threshold - with visual magnitude 6.5 the sky is about 8000 stars, and that number grows exponentially as dimmer stars are included.'''
   def start(self, pathin):
      '''initializes the sky model. first generates a skymap object and then picks out the information needed here, with some enrichment - in particular with three-dimensional vectors in the celestial reference frame.'''
   def image_generator(self, starndx):
      '''creates a standard image for the target star, ready for feeding into a star identifier. the format is 28 x 28 pixels - lo-fi, the way we like it. makes thing tougher on us. and also by no coincidence matching the classic mnist character recognition data set. the story behind that is a long one, discussed elsewhere in the project.'''
   def stars_near_point(self, x, y, z):
      ''' '''
   def stars_in_ring(self, p, radius, table):
      ''' '''
