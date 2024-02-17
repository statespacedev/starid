.. toctree::
   :maxdepth: 2
   :caption: Contents:

introduction
==================================================================================================================

we'd like to have three parallel and sometimes/often independent workflows.

- cpp - pure cpp, cpp ide and tools, folder libstarid-debug

- py - pure py, py ide and tools, folder starid

- pyext - python extension in cpp, folder libstarid

where possible, we're using classic vector and matrix representations so things look like what's in the literature. literature >> code. here's what numpy wants for row and column vectors.

.. code-block:: python

    row_vector = array([[1, 2, 3]])    # shape (1, 3)
    col_vector = array([[1, 2, 3]]).T  # shape (3, 1)

column vectors are used here - so the dot product of x with itself is xTx - or x.T @ x for numpy - shapes (1, 3)(3, 1) = (1, 1). for a (5, 3) matrix A, the product Ax or A @ x is (5, 3)(3, 1) = (5, 1), and the product ATAx or A.T @ A @ x is (3, 5)(5, 3)(3, 1) = (3, 1).

starid
==================================================================================================================

.. automodule:: starid.entrypoint_main
    :members:

sky
==================================================================================================================

.. automodule:: starid.sky.skymap
    :members:

.. automodule:: starid.sky.sky
    :members:

.. automodule:: starid.sky.geometry
    :members:

star pairs
==================================================================================================================

.. automodule:: starid.triangles.starpairs
    :members:

