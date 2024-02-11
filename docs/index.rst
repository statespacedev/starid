.. toctree::
   :maxdepth: 2
   :caption: Contents:

introduction
==================================================================================================================

we'd like to have three parallel and sometimes/often independent workflows.

- cpp - pure cpp, cpp ide and tools, folder libstarid-debug

- py - pure py, py ide and tools, folder starid

- pyext - python extension in cpp, folder libstarid

starid
==================================================================================================================

.. automodule:: starid.starid_main
    :members:

sky
==================================================================================================================

.. automodule:: starid.sky.skymap
    :members:

.. automodule:: starid.sky.sky
    :members:

.. automodule:: starid.sky.geometry
    :members:

star identification
==================================================================================================================

view the sky as triangles of stars. for the target star, it's a member of a set of triangles - eliminate candidate ids based on the geometry of these triangles. this is an iterative process and the inner loop is comparing triangle geometries. the overall speed depends on this inner loop, so the focus is on making it as efficient as possible.

identifies the target of a star image, using the triangles formed by neighboring stars within the image. the fundemental particles are actually pairs of stars - in a sense individual stars don't exist here, what exists are pairs of stars, acting as sides of triangles - so a key object handed to the identifier in its constructor is a starpairs object, containing all of the relevant pairs. when possible, the starpairs object was loaded from a cerealized starpairs file, rather than generated at run-time.

startriangleidentifier
------------------------------------------------------------------------------------------------------------------

startriangle
------------------------------------------------------------------------------------------------------------------

startriangleside
------------------------------------------------------------------------------------------------------------------
    