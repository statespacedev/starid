.. toctree::
   :maxdepth: 2
   :caption: Contents:

introduction
==================================================================================================================

we'd like to have three parallel and sometimes/often independent workflows.

- cpp - pure cpp, cpp ide and tools, folder libstarid-debug

- py - pure py, py ide and tools, folder starid

- pyext - python extension in cpp, folder libstarid

we're using classic vector and matrix representations so things look like what's in the literature. this means lots of single letter variables, with capital letters for matrices and lower case letters for vectors. literature >> code. here's what numpy wants for row and column vectors.

.. code-block:: python

    row_vector = array([[1, 2, 3]])    # shape (1, 3)
    col_vector = array([[1, 2, 3]]).T  # shape (3, 1)

column vectors are used here - so the dot product of x with itself is xTx - or x.T @ x for numpy - shapes (1, 3)(3, 1) = (1, 1). for a (5, 3) matrix A, the product Ax or A @ x is (5, 3)(3, 1) = (5, 1), and the product ATAx or A.T @ A @ x is (3, 5)(5, 3)(3, 1) = (3, 1).

starid
==================================================================================================================

.. automodule:: starid.main
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
    