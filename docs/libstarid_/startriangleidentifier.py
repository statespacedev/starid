class NOMAD:
   '''star recognition focused on a chain of triangles and baseides - side2 of each triangle is the baseside of the following triangle. during feedback, these shared side2 -> baseside pairs are the path for information to flow backwards - increasing the constraints on the initial triangle baseside and basestar.'''
   def run(self, pixels):
      '''recognize target star from the image pixels.'''
class SETTLER:
   '''identifies the target of a star image, using the triangles formed by neighboring stars within the image. the fundemental particles are actually pairs of stars - in a sense individual stars don't exist here, what exists are pairs of stars, acting as sides of triangles - so a key object handed to the identifier in its constructor is a starpairs object, containing all of the relevant pairs. when possible, the starpairs object was loaded from a cerealized starpairs file, rather than generated at run-time.'''
   def run(self, pixels):
      '''recognize target star from the image pixels.'''
   def get_angs_d(self):
      '''examine a candidate for star d before using it to form triangle abda. we want the angles from stars a, b, and c to be appreciable. the angles remain in angs_d for later use'''
   def get_angs_c(self):
      '''examine a candidate for star c before using it to form triangle abca. we want the angles between stars a, b, and c to be appreciable. the angles remain in angs_c for later use.'''
