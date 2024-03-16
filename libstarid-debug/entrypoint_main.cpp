#include <cereal/archives/binary.hpp>
#include <fstream>
#include "sky/sky.h"
#include "triangles/star_pairs.h"
#include "triangles/settler.h"
#include "triangles/nomad.h"

// use approprate path formatting, on windows C:\\Users\\i28144\\CLionProjects\\starid\\data\\skymap.txt
//static std::string dirdata = R"(C:\Users\i28144\CLionProjects\starid\data\)";
//static std::string dirtmp = R"(C:\Users\i28144\CLionProjects\starid\tmp\)";
static std::string dirdata = "../../data/";
static std::string dirtmp = "./";
static std::string pathskymap = dirdata + "skymap.txt";
static std::string pathsky = dirtmp + "sky.cereal";
static std::string pathstarpairs = dirtmp + "starpairs.cereal";

starid::Sky sky_util(std::string &pathsky, std::string &pathskymap);

starid::Starpairs starpairs_util(starid::Sky &sky, std::string &pathstarpairs);

int main() {
    auto sky = sky_util(pathsky, pathskymap);
    auto image = sky.image_generator(3);

    auto starpairs = starpairs_util(sky, pathstarpairs);

    starid::NOMAD nomad(starpairs);
    auto id2 = nomad.run(image["pixels"]);

    starid::SETTLER settler(starpairs);
    auto id1 = settler.run(image["pixels"]);

    return 0;
}

starid::Sky sky_util(std::string &pathsky_, std::string &pathskymap_) {
    starid::Sky sky;
    try {
        std::ifstream is1(pathsky_, std::ios::binary);
        cereal::BinaryInputArchive iarchive1(is1);
        iarchive1(sky);
    } catch (...) {
        sky.generate(pathskymap_);
        std::ofstream os1(pathsky_, std::ios::binary);
        cereal::BinaryOutputArchive oarchive1(os1);
        oarchive1(sky);
    }
    return sky;
}


starid::Starpairs starpairs_util(starid::Sky &sky, std::string &pathstarpairs_) {
    starid::Starpairs starpairs;
    try {
        std::ifstream is2(pathstarpairs_, std::ios::binary);
        cereal::BinaryInputArchive iarchive2(is2);
        iarchive2(starpairs);
    } catch (...) {
        starpairs.generate(sky);
        std::ofstream os2(pathstarpairs_, std::ios::binary);
        cereal::BinaryOutputArchive oarchive2(os2);
        oarchive2(starpairs);
    }
    return starpairs;
}
