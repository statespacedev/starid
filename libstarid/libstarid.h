#ifndef LIBSTARID_H
#define LIBSTARID_H
#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>
#include <Eigen/Core>
#include <cereal/archives/binary.hpp>
#include "globals.h"
#include "sky.h"
#include "triangles.h"
#include <iostream>

class libstarid {
public:
    starid::sky sky;
    starid::pairs pairs;
    libstarid();
//    void load_pairs();
//    Eigen::MatrixXd image(int starndx);
//    Eigen::MatrixXd image_info(int starndx);
//    Eigen::MatrixXd ang_seq_vec(int starndx);
//    int id(starid::image_matrix image);
};

PYBIND11_MODULE(libstarid, m) {
    pybind11::class_<libstarid>(m, "libstarid")
            .def(pybind11::init<>());
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
