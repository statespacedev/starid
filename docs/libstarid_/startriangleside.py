class Startriangleside:
   '''act as one of the three triangle sides within a parent star triangle object. here stars is a representation of candidate star pairs that could belong to the side. ultimately - when we've recognized the target star, all but one candidate star pair is eliminated.'''
   def update(self, side):
      '''used just for the abside currently being investigated, to update it based on the latest abca or abda triangle.'''
   def trim_pairs(self):
      '''eliminate candidate star pairs from the side.'''
   def pair_count(self):
      '''how many candidate star pairs remain in the side.'''
   def check_teststar(self, strndx):
      '''is the test star in the side.'''
