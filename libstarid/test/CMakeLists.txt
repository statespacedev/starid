cmake_minimum_required(VERSION 3.0)

project( test )

find_package( Eigen3 )

include_directories( ${EIGEN3_INCLUDE_DIRS} ../cereal-1.2.2/include .. )

add_executable( test test.cpp ../sky.cpp ../util.cpp ../startriangles.cpp )
