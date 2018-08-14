
cmake_minimum_required(VERSION 2.8.12)

project(libstarid)

add_subdirectory(pybind11-2.2.3)

add_subdirectory(eigen-git-mirror-3.3.5)

pybind11_add_module(libstarid libstarid.cpp)


