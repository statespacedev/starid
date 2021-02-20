class StartriangleIdentifier:
   '''identifies the target of a star image, using the triangles formed by neighboring stars within the image. the fundemental particles are actually pairs of stars - in a sense individual stars don't exist here, what exists are pairs of stars, acting as sides of triangles - so a key object handed to the identifier in its constructor is a starpairs object, containing all of the relevant pairs. when possible, the starpairs object was loaded from a cerealized starpairs file, rather than generated at run-time.'''
   def identify(self, pixels, teststar):
      '''recognizes the target of a star image, received as image pixels. the focus is on triangles that contain the target star - these are the absides, where star a is the target star, star b is a neighbor star, and an abside is a star pair and triangle side with the target as the first member of the pair. in the inner loops, two additional stars are involved - star c and star d. they form two star triangles - triangle abca, and triangle abda. when we reach an abside such that abca and abda eliminate all but one star pair possibility, we've recognized the target star.'''
   def get_angs_d(self):
      '''examine a candidate for star d before using it to form triangle abda. we want the angles from stars a, b, and c to be appreciable. the angles remain in angs_d for later use'''
   def get_angs_c(self):
      '''examine a candidate for star c before using it to form triangle abca. we want the angles between stars a, b, and c to be appreciable. the angles remain in angs_c for later use.'''
