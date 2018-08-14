
cmake_minimum_required(VERSION 3.0)

project(libstarid)

add_subdirectory(pybind11-2.2.3)
pybind11_add_module(libstarid libstarid.cpp)

#add_subdirectory(eigen-git-mirror-3.3.5)
#find_package( Eigen3 3.3 REQUIRED NO_MODULE )
#include_directories( ${EIGEN3_INCLUDE_DIRS} )
#add_library (libstarid libstarid.cpp)
#target_link_libraries (libstarid Eigen3::Eigen)


