#include <cereal/archives/binary.hpp>
#include <fstream>
#include "util-debug.h"

starid::Sky sky_util(std::string &pathsky, std::string &pathskymap) {
    starid::Sky sky;
    try {
        std::ifstream is1(pathsky, std::ios::binary);
        cereal::BinaryInputArchive iarchive1(is1);
        iarchive1(sky);
    } catch (...) {
        sky.start(pathskymap);
        std::ofstream os1(pathsky, std::ios::binary);
        cereal::BinaryOutputArchive oarchive1(os1);
        oarchive1(sky);
    }
    return sky;
}


starid::Starpairs starpairs_util(starid::Sky &sky, std::string &pathstarpairs) {
    starid::Starpairs starpairs;
    try {
        std::ifstream is2(pathstarpairs, std::ios::binary);
        cereal::BinaryInputArchive iarchive2(is2);
        iarchive2(starpairs);
    } catch (...) {
        starpairs.generate(sky);
        std::ofstream os2(pathstarpairs, std::ios::binary);
        cereal::BinaryOutputArchive oarchive2(os2);
        oarchive2(starpairs);
    }
    return starpairs;
}
