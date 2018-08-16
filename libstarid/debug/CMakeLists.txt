cmake_minimum_required(VERSION 3.0)

project(debug)

find_package( Eigen3 3.3 REQUIRED NO_MODULE )
include_directories( ${EIGEN3_INCLUDE_DIRS} ../cereal-1.2.2/include .. )

add_executable( debug debug.cpp ../sky.cpp ../util.cpp ../globals.cpp ../util.h)
