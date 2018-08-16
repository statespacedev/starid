cmake_minimum_required(VERSION 3.0)

project(libstarid)

add_subdirectory(pybind11-2.2.3)
pybind11_add_module( libstarid libstarid.cpp sky.cpp triangles.cpp util.cpp )

find_package( Eigen3 3.3 REQUIRED NO_MODULE )
include_directories( ${EIGEN3_INCLUDE_DIRS} cereal-1.2.2/include )


