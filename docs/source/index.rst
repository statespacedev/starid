.. toctree::
   :maxdepth: 2
   :caption: Contents:

introduction
==================================================================================================================

this documentation is still in initial stages - while this notification is here, it's for sure not usable. 2021-02-15

------------------------------------------------------------------------------------------------------------------

starid python - higher-level
==================================================================================================================

ideally, a lot can be done in python without knowing much about the underlying cpp - there's a model of the sky, a toolbox for working with it, and it's available via starid.py.

starid.py
------------------------------------------------------------------------------------------------------------------

python starid object making all of the lower-level stuff available - it's the interface between python and the underlying cpp. the starid object could in some sense be a singleton - there should be only one. on the other hand, it's possible to imagine paths where this is no longer true... imagine using two sets of stars from the star catalog - one including fainter stars. in short, two skies. we could have two starid objects at the same time, one for each sky.

.. automodule:: starid.starid
    :members:

------------------------------------------------------------------------------------------------------------------

libstarid cpp - lower-level
==================================================================================================================

fast inner loops for working with star triangles. also useful for working with lots of three dimensional star pointing vectors, though this is probably ok in python as well. in any case, hardware acceleration of vectorized computations, matrix and vector math via eigen, blas, and lapack. when computations become heavy, move them from python into cpp.

libstarid.cpp
------------------------------------------------------------------------------------------------------------------

.. automodule:: libstariddocs.libstarid
    :members:

sky.cpp
------------------------------------------------------------------------------------------------------------------

.. automodule:: libstariddocs.sky
    :members:

startriangles.cpp
------------------------------------------------------------------------------------------------------------------

.. automodule:: libstariddocs.startriangles
    :members:
