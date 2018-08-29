#include "libstarid.h"
#include <iostream>

libstarid::libstarid() {
    return;
}

void libstarid::write_sky(std::string pathcat, std::string pathsky) {
    starid::sky sky;
    sky.init(std::string(pathcat));
    std::ofstream os1(pathsky);
    cereal::BinaryOutputArchive oarchive1(os1);
    oarchive1(sky);
}

void libstarid::read_sky(std::string pathsky) {
    std::ifstream is1(pathsky);
    cereal::BinaryInputArchive iarchive1(is1);
    iarchive1(sky);
}

std::map<std::string, Eigen::MatrixXd> libstarid::image_generator(int starndx) {
    std::map<std::string, Eigen::MatrixXd> result = starid::sky::image_generator(starndx, sky);
    return result;
}


