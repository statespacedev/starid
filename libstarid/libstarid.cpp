#include "libstarid.h"
#include <iostream>
#include <fstream>
#include <cereal/archives/binary.hpp>

/*
 * class Libstarid:
 *    '''handles calls from libstaridpy.py'''
 * */
Libstarid::Libstarid() {
    return;
}

/*
 *    def read_sky(self):
 *       '''todo'''
 * */
void Libstarid::read_sky(std::string pathsky) {
    std::ifstream is1(pathsky);
    cereal::BinaryInputArchive iarchive1(is1);
    iarchive1(sky);
}

void Libstarid::write_sky(std::string pathsky, std::string pathcat) {
    sky.start(std::string(pathcat));
    std::ofstream os1(pathsky);
    cereal::BinaryOutputArchive oarchive1(os1);
    oarchive1(sky);
}

void Libstarid::read_starpairs(std::string pathstarpairs) {
    std::ifstream is1(pathstarpairs);
    cereal::BinaryInputArchive iarchive1(is1);
    iarchive1(starpairs);
}

void Libstarid::write_starpairs(std::string pathstarpairs) {
    starpairs.start(sky);
    std::ofstream os2(pathstarpairs);
    cereal::BinaryOutputArchive oarchive2(os2);
    oarchive2(starpairs);
}

std::map<std::string, Eigen::MatrixXd> Libstarid::image_generator(int starndx) {
    std::map<std::string, Eigen::MatrixXd> result = sky.image_generator(starndx);
    return result;
}

std::map<std::string, Eigen::MatrixXd> Libstarid::angle_generator(int starndx) {
    std::map<std::string, Eigen::MatrixXd> result = sky.angle_generator(starndx);
    return result;
}

int Libstarid::startriangles(Eigen::MatrixXd pixels) {
    starid::startriangles st(starpairs);
    return st.identify(pixels);
}

