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
    void load_sky();
    Eigen::MatrixXd image_info(int starndx);
    Eigen::MatrixXd image(int starndx);
//    starid::pairs pairs;
//    void load_pairs();
//    int id(starid::image_matrix image);
//    Eigen::MatrixXd ang_seq_vec(int starndx);
};

PYBIND11_MODULE(libstarid, m) {
    pybind11::class_<libstarid>(m, "libstarid")
            .def(pybind11::init<>())
            .def("load_sky", &libstarid::load_sky)
            .def("image_info", &libstarid::image_info)
            .def("image", &libstarid::image);
}
//PYBIND11_PLUGIN(libstarid) {
//    pybind11::module pymodule("Libstarid", "libstarid python module");
//    pybind11::class_<libstarid>(pymodule, "Libstarid")
//        .def(pybind11::init<>())
//        .def("image", &libstarid::image, "create star image", pybind11::arg("starndx"))
//        .def("image_info", &libstarid::image_info, "create star image info", pybind11::arg("starndx"))
//        .def("ang_seq_vec", &libstarid::ang_seq_vec, "create angle sequence vector", pybind11::arg("starndx"))
//        .def("load_pairs", &libstarid::load_pairs, "load pairs")
//        .def("id", &libstarid::id, "id");
//    return pymodule.ptr();
//}


#endif
