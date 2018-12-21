cmake_minimum_required(VERSION 3.0)

project( testrunner )

find_package( Eigen3 )

include_directories( ${EIGEN3_INCLUDE_DIRS} ../cereal-1.2.2/include .. )

add_executable( testrunner testrunner.cpp ../sky.cpp ../util.cpp ../startriangles.cpp )
