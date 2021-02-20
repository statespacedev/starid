#ifndef LIBSTARID_H
#define LIBSTARID_H
#include <Eigen/Core>
#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>
#include <pybind11/stl.h>
#include "sky.h"
#include "startrianglestarid.h"
#include "starpairs.h"

class Api {
public:
    Api();
    starid::Sky sky;
    starid::Starpairs starpairs;
    void read_sky(std::string pathsky);
    void write_sky(std::string pathsky, std::string pathcat);
    void read_starpairs(std::string pathstarpairs);
    void write_starpairs(std::string pathstarpairs);
    std::map<std::string, Eigen::MatrixXd> image_generator(int starndx);
    int image_identifier(Eigen::MatrixXd pixels);
};

PYBIND11_MODULE(libstarid, m) {
    pybind11::class_<Api>(m, "Api")
            .def(pybind11::init<>())
            .def("write_sky", &Api::write_sky, pybind11::arg("pathsky"), pybind11::arg("pathcat"))
            .def("write_starpairs", &Api::write_starpairs, pybind11::arg("pathstarpairs"))
            .def("read_sky", &Api::read_sky, pybind11::arg("pathsky"))
            .def("read_starpairs", &Api::read_starpairs, pybind11::arg("pathstarpairs"))
            .def("image_generator", &Api::image_generator)
            .def("image_identifier", &Api::image_identifier, pybind11::arg("pixels"));
}

#endif
