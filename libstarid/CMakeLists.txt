
cmake_minimum_required( VERSION 3.0 )

project( libstarid )
find_package( Eigen3 )
include_directories( ${EIGEN3_INCLUDE_DIRS} cereal/include /usr/local/include /usr/include/mkl )
link_directories( /usr/local/lib /usr/lib )

set( CMAKE_CXX_STANDARD 14 )
set( CMAKE_CXX_FLAGS "-mavx -mfma -openmp -DEIGEN_USE_MKL_ALL" )
set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Ofast" )
set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast" )

add_subdirectory( pybind11 )

pybind11_add_module( libstarid api.cpp skymap.cpp sky.cpp starpairs.cpp startriangle.cpp startriangleside.cpp startriangleidentifier.cpp util.cpp )

target_link_libraries( libstarid PRIVATE blas lapack )
