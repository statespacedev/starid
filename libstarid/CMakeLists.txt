cmake_minimum_required(VERSION 3.12)

project(libstarid)

add_subdirectory(include/pybind11)
pybind11_add_module(libstarid api.cpp skymap.cpp sky.cpp starpairs.cpp startriangle.cpp startriangleside.cpp startriangleidentifier.cpp util.cpp)
target_include_directories(libstarid PRIVATE include)
