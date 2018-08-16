cmake_minimum_required(VERSION 3.0)

project(libstarid-test)

find_package( Eigen3 3.3 REQUIRED NO_MODULE )
include_directories( ${EIGEN3_INCLUDE_DIRS} ../cereal-1.2.2/include ..)

add_executable(libstarid-test libstarid-test.cpp ../globals.cpp ../sky.cpp)
