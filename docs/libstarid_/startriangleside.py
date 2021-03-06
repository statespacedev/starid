class Startriangleside:
   '''act as one of the three triangle sides within a parent star triangle object. here stars is a representation of candidate star pairs that could belong to the side. ultimately - when we've recognized the target star, all but one candidate star pair is eliminated.'''
   def update(self, side):
      '''used just for the abside currently being investigated, to update it based on the latest abca or abda triangle.'''
   def drops(self):
      '''there's a pairhalf1 -> pairhalf -> 0 or 1 concept. 0 is the default and means drop this particular pair. here we drop all pairs that have not been set to 1, and reset all that remain to 0.'''
   def countpairs(self):
      '''how many candidate star pairs remain in the side.'''
