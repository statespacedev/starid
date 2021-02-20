.. toctree::
   :maxdepth: 2
   :caption: Contents:

introduction
==================================================================================================================

this documentation is still in initial stages - while this notification is here, it's for sure not usable. 2021-02-15

------------------------------------------------------------------------------------------------------------------

starid python
==================================================================================================================

ideally, a lot can be done in python without knowing much about the underlying cpp - there's a model of the sky, a toolbox for working with it, and it's available via starid.py.

starid.py
------------------------------------------------------------------------------------------------------------------

python starid object making all of the lower-level stuff available - it's the interface between python and the underlying cpp. the starid object could in some sense be a singleton - there should be only one. on the other hand, it's possible to imagine paths where this is no longer true... imagine using two sets of stars from the star catalog - one including fainter stars. in short, two skies. we could have two starid objects at the same time, one for each sky.

.. automodule:: starid.starid
    :members:

------------------------------------------------------------------------------------------------------------------

libstarid cpp
==================================================================================================================

fast inner loops for working with star triangles. also useful for working with lots of three-dimensional star pointing vectors, though this is probably reasonable in python as well. in any case, hardware acceleration of vectorized computations, matrix and vector math via eigen. when computations become heavier, move them from python into cpp.

api.cpp
------------------------------------------------------------------------------------------------------------------

provides the services used by higher-level python code. starid.py is a collection of calls to this api - a collection that happens to be able to make things happen and get things done. there's no reason other collections of call from python couldn't evolve as well.

.. automodule:: libstarid_.api
    :members:

skymap.cpp
------------------------------------------------------------------------------------------------------------------

.. automodule:: libstarid_.skymap
    :members:

sky.cpp
------------------------------------------------------------------------------------------------------------------

interactive model of the sky, based on a set of stars from the nasa skymap star catalog. the stars are defined by a brightness cutoff - all stars brighter than the cutoff. with a cutoff of visual magnitude 6.5, this means slightly more than all stars visible to human eyes - 8876 in total.

.. automodule:: libstarid_.sky
    :members:

starpairs.cpp
------------------------------------------------------------------------------------------------------------------

.. automodule:: libstarid_.starpairs
    :members:

startriangleside.cpp
------------------------------------------------------------------------------------------------------------------

.. automodule:: libstarid_.startriangleside
    :members:

startriangle.cpp
------------------------------------------------------------------------------------------------------------------

.. automodule:: libstarid_.startriangle
    :members:

startrianglestarid.cpp
------------------------------------------------------------------------------------------------------------------

view the sky as triangles of stars. for the target star, it's a member of a set of triangles - eliminate candidate ids based on the geometry of these triangles. this is an iterative process and the inner loop is comparing triangle geometries. the overall speed depends on this inner loop, so the focus is on making it as efficient as possible.

.. automodule:: libstarid_.startriangles
    :members:



