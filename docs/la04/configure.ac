AC_INIT(la04, 1.2.0, hsl@stfc.ac.uk)
AC_CONFIG_SRCDIR([src/la04d.f])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE

# Check for required programs
AC_PROG_FC
# Set f77 compiler to be same as f90 compiler unless explcitly told otherwise
if test "x$F77" = "x"; then
   F77="$FC"
fi
if test "x$FFLAGS" = "x"; then
   FFLAGS="$FCFLAGS"
fi
AC_PROG_F77
AC_PROG_CC
AC_PROG_LIBTOOL

# Establish some required flags
AC_FC_LIBRARY_LDFLAGS # Linking C to Fortran


# Establish correct module search path flag
AX_F90_MODULE_FLAG
# Check if we're on Windows and have to do extra legwork for DLLs
AC_CHECK_HEADERS(windows.h)
AM_CONDITIONAL(TARGET_DLL, test x$ac_cv_header_windows_h = xyes)

# Check for external libraries (e.g. BLAS, LAPACK, MeTiS)

# Output data
AC_CONFIG_FILES(Makefile src/Makefile example/Makefile)
AC_OUTPUT
