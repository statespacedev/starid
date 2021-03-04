#include "api.h"
#include <iostream>
#include <fstream>
#include <cereal/archives/binary.hpp>

/*
 * class Api:
 *    '''handles calls from python code, for example starid.py. where reading and writing of data files is done, the headers-only cereal library is used. this is the cpp version of the python pickle library. binary objects are moved directly to and from disk. binary data is more efficient - it's smaller and faster.'''
 * */
Api::Api() {}

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
 *       '''generate a sky object and write a sky data file using cereal. generating a sky object from scratch can take a noticeable amount of time - a few seconds?'''
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

/*
 *    def write_starpairs(self):
 *       '''generate star pairs for a given set of stars and write a starpair data file using cereal. this scales up quickly with star density on the sky - so with the brightness threshold we're using. including fainter stars increases the density exponentially, along with the computational cost of generating star pairs. for each star, we only care about its neighbors that can appear in the images we're using. bigger images mean more neighbors to generate star pairs for. our current baseline is visual magnitude 6.5 - the roughly eight thousand stars visible to the human eye - and an image size of eight by eight degrees, typical for a certain generation of star trackers.'''
 * */
void Api::write_starpairs(std::string pathstarpairs) {
    starpairs.generate(sky);
    std::ofstream os2(pathstarpairs);
    cereal::BinaryOutputArchive oarchive2(os2);
    oarchive2(starpairs);
}

/*
 *    def image_generator(self, starndx):
 *       '''for the star indicated by starndx, generate a standard lo-fi image, with the sky randomly rotated. this is an image for which we want to perform star identification. it's the input to the image_identifier method, which performs identification and outputs the resulting id - if that matches the starndx, identification was a success.'''
 * */
std::map<std::string, Eigen::MatrixXd> Api::image_generator(int starndx) {
    std::map<std::string, Eigen::MatrixXd> result = sky.image_generator(starndx);
    return result;
}

/*
 *    def SETTLER(self, image_pixels):
 *       '''performs identification and outputs an id - if that matches the starndx used by image_generator to create the image, identification was a success.'''
 * */
int Api::SETTLER(Eigen::MatrixXd pixels) {
    starid::SETTLER settler(starpairs);
    return settler.run(pixels);
}

/*
 *    def NOMAD(self, image_pixels):
 *       '''performs identification and outputs an id - if that matches the starndx used by image_generator to create the image, identification was a success.'''
 * */
int Api::NOMAD(Eigen::MatrixXd pixels) {
    starid::NOMAD nomad(starpairs);
    return nomad.run(pixels);
}