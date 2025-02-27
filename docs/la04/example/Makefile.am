AUTOMAKE_OPTIONS = serial-tests
AM_FCFLAGS = -I $(top_builddir)/src

TESTS_ENVIRONMENT = $(srcdir)/runtest.sh
EXTRA_DIST = runtest.sh
TESTS = \
	la04ss \
	la04ds

check_PROGRAMS = \
	la04ss \
	la04ds

la04ss_SOURCES = la04ss.f
la04ss_LDADD = $(top_builddir)/src/libla04.la $(LAPACK_LIBS) $(BLAS_LIBS) $(METIS_LIBS) $(FCLIBS)
EXTRA_DIST += la04ss.output la04ss.data
la04ds_SOURCES = la04ds.f
la04ds_LDADD = $(top_builddir)/src/libla04.la $(LAPACK_LIBS) $(BLAS_LIBS) $(METIS_LIBS) $(FCLIBS)
EXTRA_DIST += la04ds.output la04ds.data
