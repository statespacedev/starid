#include "libstarid.h"
#include <iostream>
#include <fstream>
#include <cereal/archives/binary.hpp>

libstarid::libstarid() {
    return;
}

void libstarid::write_sky(std::string pathsky, std::string pathcat) {
    starid::sky sky;
    sky.write_sky(std::string(pathcat));
    std::ofstream os1(pathsky);
    cereal::BinaryOutputArchive oarchive1(os1);
    oarchive1(sky);
}

void libstarid::write_starpairs(std::string pathstarpairs, std::string pathsky) {
    std::ifstream is1(pathsky);
    cereal::BinaryInputArchive iarchive1(is1);
    iarchive1(sky);
    starid::starpairs pairs;
    pairs.write_starpairs(sky);
    std::ofstream os2(pathstarpairs);
    cereal::BinaryOutputArchive oarchive2(os2);
    oarchive2(pairs);
}

void libstarid::read_sky(std::string pathsky) {
    std::ifstream is1(pathsky);
    cereal::BinaryInputArchive iarchive1(is1);
    iarchive1(sky);
}

void libstarid::read_starpairs(std::string pathstarpairs) {
    starid::starpairs starpairs;
    std::ifstream is1(pathstarpairs);
    cereal::BinaryInputArchive iarchive1(is1);
    iarchive1(starpairs);
}

std::map<std::string, Eigen::MatrixXd> libstarid::image_generator(int starndx) {
    std::map<std::string, Eigen::MatrixXd> result = sky.image_generator(starndx);
    return result;
}

std::map<std::string, Eigen::MatrixXd> libstarid::angle_generator(int starndx) {
    std::map<std::string, Eigen::MatrixXd> result = sky.angle_generator(starndx);
    return result;
}
