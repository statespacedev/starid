cmake_minimum_required(VERSION 3.0)
project(libstarid)

find_package( Eigen3 )
include_directories( ${EIGEN3_INCLUDE_DIRS} cereal/include)
add_subdirectory( pybind11 )

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-mavx -mfma")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast")
pybind11_add_module( libstarid api.cpp skymap.cpp sky.cpp starpairs.cpp startriangle.cpp startriangleside.cpp startriangleidentifier.cpp util.cpp )
