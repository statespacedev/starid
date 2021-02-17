#include "api.h"
#include <iostream>
#include <fstream>
#include <cereal/archives/binary.hpp>

/*
 * class Api:
 *    '''handles calls from python code, for example starid.py. where reading and writing of data files is done, the headers-only cereal library is used. this is the cpp version of the python pickle library. binary objects are moved directly to and from disk. binary data is more efficient - it's smaller and faster.'''
 * */
Api::Api() {
    return;
}

/*
 *    def read_sky(self):
 *       '''start a sky object from a sky data file using cereal. the key part of sky data is a three-dimensional pointing vector representing the direction to each star in the celestial reference frame.'''
 * */
void Api::read_sky(std::string pathsky) {
    std::ifstream is1(pathsky);
    cereal::BinaryInputArchive iarchive1(is1);
    iarchive1(sky);
}

/*
 *    def write_sky(self):
 *       '''generate a sky object and write a sky data file using cereal. generating a sky object from scratch can take a while - tens of seconds?'''
 * */
void Api::write_sky(std::string pathsky, std::string pathcat) {
    sky.start(std::string(pathcat));
    std::ofstream os1(pathsky);
    cereal::BinaryOutputArchive oarchive1(os1);
    oarchive1(sky);
}

/*
 *    def read_starpairs(self):
 *       '''start a starpair object from a starpair data file using cereal. this object is the key ingredient for star triangles - a star pair is a triangle-side in star triangles, it's the 'fundamental particle' of a triangle view of the sky. every triangle is made of three starpairs. in a starpair object, for each star, the starpairs with each of its near neighbors are represented, pre-computed and ready for use. computing this object is relatively heavy and we want to do it once, in advance, and then reuse it from there.'''
 * */
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

