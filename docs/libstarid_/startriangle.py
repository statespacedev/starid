class StartriangleNOMAD:
   '''NOMAD triangle. focus is on the basestar and baseside - nomad is about a chain of basesides, each increasing the constraints on the preceding basestars. closing the loop around a triangle for each baseside speeds up that process.'''
   def isrecognized(self):
      '''true if basestars and basesides have been constrained to the point where only one possible basestar remains.'''
class StartriangleSETTLER:
   '''SETTLER triangle. acts as the triangles abca and abda within the star triangle identifier inner loops. their are three triangle sides - representing three star pairs, each with an angular separation. each side is acted by a star triangle side object.'''
   def close_loops_abda(self, triangles):
      '''test candidate star pairs for the sides of an abda triangle.'''
   def close_loops_abca(self):
      '''test candidate star pairs for the sides of an abca triangle.'''
