#ifndef LIBSTARID_H
#define LIBSTARID_H
#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>
#include <Eigen/Core>
#include <cereal/archives/binary.hpp>
#include "sky.h"
#include "triangles.h"
#include <iostream>

class libstarid {
    
public:
    libstarid();
    starid::sky sky;
    void write_sky(std::string pathcat, std::string pathsky);
    void read_sky(std::string pathsky);
    Eigen::MatrixXd image_info(int starndx);
    Eigen::MatrixXd image(int starndx);
};

PYBIND11_MODULE(libstarid, m) {
    pybind11::class_<libstarid>(m, "libstarid")
            .def(pybind11::init<>())
            .def("write_sky", &libstarid::write_sky, pybind11::arg("pathcat"), pybind11::arg("pathsky"))
            .def("read_sky", &libstarid::read_sky, pybind11::arg("pathsky"))
            .def("image_info", &libstarid::image_info)
            .def("image", &libstarid::image);
}

#endif
