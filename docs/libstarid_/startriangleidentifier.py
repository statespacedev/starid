class NOMAD:
   '''star recognition focused on a chain of triangles and basesides - side2 of each triangle is the baseside of the following triangle. during feedback, these shared side2 -> baseside pairs are the path for information to flow backwards - increasing the constraints on the initial triangle baseside and basestar. the name NOMAD relates to how the chain of triangles wanders away from the target star and initial triangle.'''
   def run(self, pixels):
      '''recognize target star from the image pixels.'''
class SETTLER:
   '''the target star ia always star a. star b is a neighbor star, and an abside is a star pair and triangle side with the target as the first member of the pair. in the inner loops, additional stars c and d are involved. first an abca triangle is formed. this constrains the abside. then for an abca triangle, a sequence of abda triangles are formed, further constraining the abside. when we reach an abda that eliminates all but one star pair possibility for the abside, we've recognized the target star. the name SETTLER comes from the idea that we never move away the target star, we're settling around it.'''
   def run(self, pixels):
      '''recognize target star from the image pixels.'''
   def get_angs_d(self):
      '''examine a candidate for star d before using it to form triangle abda. we want the angles from stars a, b, and c to be appreciable. the angles remain in angs_d for later use'''
   def get_angs_c(self):
      '''examine a candidate for star c before using it to form triangle abca. we want the angles between stars a, b, and c to be appreciable. the angles remain in angs_c for later use.'''
