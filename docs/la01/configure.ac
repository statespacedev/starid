AC_INIT(la01, 1.0.0, hsl@stfc.ac.uk)
AC_CONFIG_SRCDIR([src/la01s.f])
AM_INIT_AUTOMAKE(la01, 1.0.0)

# Check for required programs
AC_PROG_F77
AC_PROG_FC
AC_PROG_CC
AC_PROG_RANLIB

# Establish some required flags
AC_FC_LIBRARY_LDFLAGS # Linking C to Fortran

# Establish correct module search path flag
AX_F90_MODULE_FLAG

# Check for external libraries (e.g. BLAS, LAPACK, MeTiS)

# Output data
AC_CONFIG_FILES(Makefile src/Makefile example/Makefile)
AC_OUTPUT
