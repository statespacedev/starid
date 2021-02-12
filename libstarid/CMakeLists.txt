cmake_minimum_required(VERSION 3.0)
project(libstarid)

find_package( Eigen3 )

add_subdirectory( pybind11 )

pybind11_add_module( libstarid libstarid.cpp sky.cpp startriangles.cpp util.cpp )

include_directories( ${EIGEN3_INCLUDE_DIRS} cereal/include)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -mavx -mfma")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Ofast")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Ofast")
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
#set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3")
