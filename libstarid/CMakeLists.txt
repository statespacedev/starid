cmake_minimum_required(VERSION 3.0)

project(libstarid)

find_package( Eigen3 )

add_subdirectory(pybind11-2.2.3)

pybind11_add_module( libstarid libstarid.cpp sky.cpp startriangles.cpp util.cpp )

include_directories( ${EIGEN3_INCLUDE_DIRS} cereal-1.2.2/include )


