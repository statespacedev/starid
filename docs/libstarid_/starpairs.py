class Starpairs:
   '''foundation for star triangles - a star pair is a triangle-side - it's the fundamental particle of a triangle view of the sky. every triangle is made of three starpairs. in a starpair object, for each star, the starpairs with each of its near neighbors are represented, pre-computed and ready for use. computing this object is relatively heavy and we want to do it once, in advance, and then reuse it from there. so cerealize it to a starpairs file and read in the starpairs object from that whenever possible, rather than generating from scratch.'''
   def generate(self, sky):
      ''' '''
   def pairsndxr(self, angle, tol_radius):
      ''' '''
   def pairlabeler(self, catndx1, catndx2):
      ''' '''
