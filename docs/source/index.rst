.. toctree::
   :maxdepth: 2
   :caption: Contents:

introduction
==================================================================================================================

code is split into two folders, with the starid folder playing a special role as a python package for release on the pypi package repo.

starid python
------------------------------------------------------------------------------------------------------------------

ideally, a lot can be done in python without knowing much about the underlying cpp - there's a model of the sky, a toolbox for working with it, and it's available via starid.py.

libstarid cpp
------------------------------------------------------------------------------------------------------------------

fast inner loops for working with star triangles. also useful for working with lots of three-dimensional star pointing vectors, though this is probably reasonable in python as well. in any case, hardware acceleration of vectorized computations, matrix and vector math via eigen. when computations become heavier, move them from python into cpp.

install
------------------------------------------------------------------------------------------------------------------

pypi pip-install is mostly a placeholder for now, gitlab-repo clone-install is the baseline. the following should work on ubuntu.

    sudo apt install cmake libeigen3-dev
    git clone git@gitlab.com:noahhsmith/starid.git starid
    cd starid
    python3 -m venv venv
    . venv/bin/activate
    pip3 install .
    python3 starid --demo

higher-level
==================================================================================================================

python starid object making all of the lower-level stuff available - it's the interface between python and the underlying cpp. the starid object could in some sense be a singleton - there should be only one. on the other hand, it's possible to imagine paths where this is no longer true... imagine using two sets of stars from the star catalog - one including fainter stars. in short, two skies. we could have two starid objects at the same time, one for each sky.

starid.py
------------------------------------------------------------------------------------------------------------------

.. automodule:: starid.starid
    :members:

api.cpp
------------------------------------------------------------------------------------------------------------------

.. automodule:: libstarid_.api
    :members:

sky model
==================================================================================================================

interactive model of the sky, based on a set of stars from the nasa skymap star catalog. the stars are defined by a brightness cutoff - all stars brighter than the cutoff. with a cutoff of visual magnitude 6.5, this means slightly more than all stars visible to human eyes - 8876 in total.

skymap.cpp
------------------------------------------------------------------------------------------------------------------

.. automodule:: libstarid_.skymap
    :members:

sky.cpp
------------------------------------------------------------------------------------------------------------------

.. automodule:: libstarid_.sky
    :members:

starpairs.cpp
------------------------------------------------------------------------------------------------------------------

.. automodule:: libstarid_.starpairs
    :members:

identification
==================================================================================================================

view the sky as triangles of stars. for the target star, it's a member of a set of triangles - eliminate candidate ids based on the geometry of these triangles. this is an iterative process and the inner loop is comparing triangle geometries. the overall speed depends on this inner loop, so the focus is on making it as efficient as possible.

identifies the target of a star image, using the triangles formed by neighboring stars within the image. the fundemental particles are actually pairs of stars - in a sense individual stars don't exist here, what exists are pairs of stars, acting as sides of triangles - so a key object handed to the identifier in its constructor is a starpairs object, containing all of the relevant pairs. when possible, the starpairs object was loaded from a cerealized starpairs file, rather than generated at run-time.

startriangleidentifier.cpp
------------------------------------------------------------------------------------------------------------------

.. automodule:: libstarid_.startriangleidentifier
    :members:

startriangle.cpp
------------------------------------------------------------------------------------------------------------------

.. automodule:: libstarid_.startriangle
    :members:

startriangleside.cpp
------------------------------------------------------------------------------------------------------------------

.. automodule:: libstarid_.startriangleside
    :members:



