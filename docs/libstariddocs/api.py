class Api:
   '''handles calls from python code, for example starid.py. where reading and writing of data files is done, the headers-only cereal library is used. this is the cpp version of the python pickle library. binary objects are moved directly to and from disk. binary data is more efficient - it's smaller and faster.'''
   def read_sky(self):
      '''start a sky object from a sky data file using cereal. the key part of sky data is a three-dimensional pointing vector representing the direction to each star in the celestial reference frame.'''
   def write_sky(self):
      '''generate a sky object and write a sky data file using cereal. generating a sky object from scratch can take a while - tens of seconds?'''
   def read_starpairs(self):
      '''start a starpair object from a starpair data file using cereal. this object is the key ingredient for star triangles - a star pair is a triangle-side in star triangles, it's the 'fundamental particle' of a triangle view of the sky. every triangle is made of three starpairs. in a starpair object, for each star, the starpairs with each of its near neighbors are represented, pre-computed and ready for use. computing this object relatively heavy and we want to do it once, in advance, and then reuse it from there.'''
