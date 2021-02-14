#ifndef LIBSTARID_H
#define LIBSTARID_H
#include <Eigen/Core>
#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>
#include <pybind11/stl.h>
#include "sky.h"
#include "startriangles.h"

class Libstarid {
public:
    Libstarid();
    starid::sky sky;
    starid::starpairs starpairs;
    void read_sky(std::string pathsky);
    void write_sky(std::string pathsky, std::string pathcat);
    void read_starpairs(std::string pathstarpairs);
    void write_starpairs(std::string pathstarpairs);
    std::map<std::string, Eigen::MatrixXd> image_generator(int starndx);
    std::map<std::string, Eigen::MatrixXd> angle_generator(int starndx);
    int startriangles(Eigen::MatrixXd pixels);
};

PYBIND11_MODULE(libstarid, m) {
    pybind11::class_<Libstarid>(m, "Libstarid")
            .def(pybind11::init<>())
            .def("write_sky", &Libstarid::write_sky, pybind11::arg("pathsky"), pybind11::arg("pathcat"))
            .def("write_starpairs", &Libstarid::write_starpairs, pybind11::arg("pathstarpairs"))
            .def("read_sky", &Libstarid::read_sky, pybind11::arg("pathsky"))
            .def("read_starpairs", &Libstarid::read_starpairs, pybind11::arg("pathstarpairs"))
            .def("image_generator", &Libstarid::image_generator)
            .def("angle_generator", &Libstarid::angle_generator)
            .def("startriangles", &Libstarid::startriangles, pybind11::arg("pixels"));
}

#endif
