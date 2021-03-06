class StartriangleNOMAD:
   '''NOMAD triangle. focus is on the basestar and baseside - nomad is about a chain of basesides, each increasing the constraints on the preceding basestars. closing the loop around a triangle for each baseside speeds up that process.'''
   def constrain(self):
      '''in each of the three sides, there's a pairhalf1 -> pairhalf -> 0 or 1 concept. 0 is the default and means drop this pair. here we will mark pairs to keep by setting them to 1, all others will be dropped.'''
   def backflow(self):
      '''increase the constraints on the baseside in the prev triangle, using the baseside of the next triangle in the chain. as triangles are added, constraints flow backwards through preceding basesides and basestars.'''
   def isrecognized(self):
      '''true if basestars and basesides have been constrained to the point where only one possible basestar remains.'''
class StartriangleSETTLER:
   '''SETTLER triangle. acts as the triangles abca and abda within the star triangle identifier inner loops. their are three triangle sides - representing three star pairs, each with an angular separation. each side is acted by a star triangle side object.'''
   def constrain_abda(self, triangles):
      '''test candidate star pairs for the sides of an abda triangle.'''
   def constrain_abca(self):
      '''test candidate star pairs for the sides of an abca triangle.'''
