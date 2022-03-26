
cmake_minimum_required( VERSION 3.0 )

project( libstarid )
find_package( Eigen3 )

include_directories( ${EIGEN3_INCLUDE_DIRS} /usr/local/include )
link_directories( /usr/local/lib /usr/lib )

add_subdirectory( /tmp/pybind11 cmake-build-debug )
pybind11_add_module( libstarid api.cpp skymap.cpp sky.cpp starpairs.cpp startriangle.cpp startriangleside.cpp startriangleidentifier.cpp util.cpp )
