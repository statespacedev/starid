.. toctree::
  :maxdepth: 2
  :hidden:

  introduction
  processors
  models
  notes

statespace
=============
this project focuses on numpy implementations of kalman, sigma point, and particle - using classic vector and matrix representations so things look like what's in the literature. this means lots of single letter variables, with capital letters for matrices and lower case letters for vectors. literature >> code. here's what numpy wants for row and column vectors.

.. code-block:: python
    
    row_vector = array([[1, 2, 3]])    # shape (1, 3)
    col_vector = array([[1, 2, 3]]).T  # shape (3, 1)

column vectors are used here - so the dot product of x with itself is xTx - or x.T @ x for numpy - shapes (1, 3)(3, 1) = (1, 1). for a (5, 3) matrix A, the product Ax or A @ x is (5, 3)(3, 1) = (5, 1), and the product ATAx or A.T @ A @ x is (3, 5)(5, 3)(3, 1) = (3, 1).

full docs-as-code is also an experimental objective - and having some fun preserving a kind of historical perspective.
