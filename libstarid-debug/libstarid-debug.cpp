#include <fstream>
#include <Eigen/Core>
#include <cereal/archives/binary.hpp>
#include "util.h"
#include "sky.h"
#include "startriangleidentifier.h"
#include "starpairs.h"

// use approprate path formatting, on windows C:\\Users\\i28144\\CLionProjects\\starid\\data\\cat
static std::string dirdata = "C:\\Users\\i28144\\CLionProjects\\starid\\data\\";
static std::string dirtmp = "C:\\Users\\i28144\\CLionProjects\\starid\\tmp\\";
static std::string pathcat = dirdata + "cat";
static std::string pathsky = dirtmp + "sky";
static std::string pathstarpairs = dirtmp + "starpairs";

starid::Sky read_sky();

starid::Starpairs read_starpairs(starid::Sky &sky);

int main() {
    auto sky = read_sky();
    auto starpairs = read_starpairs(sky);
    auto image = sky.image_generator(3);
    starid::SETTLER settler(starpairs);
    auto id1 = settler.run(image["pixels"]);
    starid::NOMAD nomad(starpairs);
    auto id2 = nomad.run(image["pixels"]);
    return 0;
}

starid::Sky read_sky() {
    starid::Sky sky;
    try {
        std::ifstream is1(pathsky, std::ios::binary);
        cereal::BinaryInputArchive iarchive1(is1);
        iarchive1(sky);
    } catch (...) {
        sky.start(pathcat);
        std::ofstream os1(pathsky, std::ios::binary);
        cereal::BinaryOutputArchive oarchive1(os1);
        oarchive1(sky);
    }
    return sky;
}

starid::Starpairs read_starpairs(starid::Sky &sky) {
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
