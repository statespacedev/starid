class Starpairs:
   '''foundation for star triangles - a star pair is a triangle-side - it's the fundamental particle of a triangle view of the sky. every triangle is made of three starpairs. in a starpair object, for each star, the starpairs with each of its near neighbors are represented, pre-computed and ready for use. computing this object is relatively heavy and we want to do it once, in advance, and then reuse it from there. so cerealize it to a starpairs file and read in the starpairs object from that whenever possible, rather than generating from scratch.'''
   def generate(self, sky):
      '''create a starpairs object from scratch. this can written to disk using cereal, and read from there in the future to bypass these computations.'''
   def pairs_for_angle(self, angle, tol_radius):
      '''for an angle, what are the candidate star pairs? creates the representation of stars used in star triangles. there, a star is a collection of associations with its near neighbors - its essential feature is its membership in pairs and triangle sides. what we do here is look at each star in turn, asking the question - what pairings do we care about for the star triangle representation of the sky we're going to use? the tuning parameters representing the answer to that question are the angle between pair members and a measure of tolerance or sensitivity.'''
   def pair_labeler(self, catndx1, catndx2):
      '''returns a unique string for the pair, consisting of the catalog ids for the member stars - a useful identifier for the pair.'''
