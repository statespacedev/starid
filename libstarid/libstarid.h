#ifndef LIBSTARID_H
#define LIBSTARID_H
#include <Eigen/Core>
#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>
#include <pybind11/stl.h>
#include "sky.h"
#include "startriangles.h"

class libstarid {
public:
    libstarid();
    
    starid::sky sky;
    void read_sky(std::string pathsky);
    void write_sky(std::string pathsky, std::string pathcat);

    starid::starpairs starpairs;
    void read_starpairs(std::string pathstarpairs);
    void write_starpairs(std::string pathstarpairs);
    
    std::map<std::string, Eigen::MatrixXd> image_generator(int starndx);
    std::map<std::string, Eigen::MatrixXd> angle_generator(int starndx);
    int triangles(Eigen::MatrixXd pixels);
};

PYBIND11_MODULE(libstarid, m) {
    pybind11::class_<libstarid>(m, "libstarid")
            .def(pybind11::init<>())
            .def("write_sky", &libstarid::write_sky, pybind11::arg("pathsky"), pybind11::arg("pathcat"))
            .def("write_starpairs", &libstarid::write_starpairs, pybind11::arg("pathstarpairs"))
            .def("read_sky", &libstarid::read_sky, pybind11::arg("pathsky"))
            .def("read_starpairs", &libstarid::read_starpairs, pybind11::arg("pathstarpairs"))
            .def("image_generator", &libstarid::image_generator)
            .def("angle_generator", &libstarid::angle_generator)
            .def("triangles", &libstarid::triangles, pybind11::arg("pixels"));
}

#endif
