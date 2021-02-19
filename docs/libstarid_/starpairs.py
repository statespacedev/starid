class Starpairs:
   '''acts as the key ingredient for star triangles - a star pair is a triangle-side in star triangles, it's the 'fundamental particle' of a triangle view of the sky. every triangle is made of three starpairs. in a starpair object, for each star, the starpairs with each of its near neighbors are represented, pre-computed and ready for use. computing this object is relatively heavy and we want to do it once, in advance, and then reuse it from there. so cerealize it to a starpairs file and start from that whenever possible.'''
   def pairsndxr(self, angle, tol_radius):
      ''' '''
   def pairlabeler(self, catndx1, catndx2):
      ''' '''
