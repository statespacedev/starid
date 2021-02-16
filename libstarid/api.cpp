#include "api.h"
#include <iostream>
#include <fstream>
#include <cereal/archives/binary.hpp>

/*
 * class Api:
 *    '''handles calls from python code, for example starid.py.'''
 * */
Api::Api() {
    return;
}

/*
 *    def read_sky(self):
 *       '''start a sky object from a sky data file.'''
 * */
void Api::read_sky(std::string pathsky) {
    std::ifstream is1(pathsky);
    cereal::BinaryInputArchive iarchive1(is1);
    iarchive1(sky);
}

/*
 *    def write_sky(self):
 *       '''generate a sky object and write a sky data file. generating a sky object from scratch can take a while - tens of seconds?'''
 * */
void Api::write_sky(std::string pathsky, std::string pathcat) {
    sky.start(std::string(pathcat));
    std::ofstream os1(pathsky);
    cereal::BinaryOutputArchive oarchive1(os1);
    oarchive1(sky);
}

void Api::read_starpairs(std::string pathstarpairs) {
    std::ifstream is1(pathstarpairs);
    cereal::BinaryInputArchive iarchive1(is1);
    iarchive1(starpairs);
}

void Api::write_starpairs(std::string pathstarpairs) {
    starpairs.start(sky);
    std::ofstream os2(pathstarpairs);
    cereal::BinaryOutputArchive oarchive2(os2);
    oarchive2(starpairs);
}

std::map<std::string, Eigen::MatrixXd> Api::image_generator(int starndx) {
    std::map<std::string, Eigen::MatrixXd> result = sky.image_generator(starndx);
    return result;
}

std::map<std::string, Eigen::MatrixXd> Api::angle_generator(int starndx) {
    std::map<std::string, Eigen::MatrixXd> result = sky.angle_generator(starndx);
    return result;
}

int Api::startriangles(Eigen::MatrixXd pixels) {
    starid::Startriangles st(starpairs);
    return st.identify(pixels);
}

