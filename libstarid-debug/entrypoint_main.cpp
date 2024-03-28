#include <cereal/archives/binary.hpp>
#include <fstream>
#include "sky/sky.h"
#include "triangles/star_pairs.h"
#include "triangles/settler.h"
#include "triangles/nomad.h"
using namespace starid;
using namespace std;
static string dirdata = "../../data/";
static string dirtmp = "./";
static string pathskymap = dirdata + "skymap.txt";
static string pathsky = dirtmp + "sky.cereal";
static string pathstarpairs = dirtmp + "starpairs.cereal";
Sky sky_util(string &pathsky, string &pathskymap);
Starpairs starpairs_util(Sky &sky, string &pathstarpairs);

int main() {
    auto sky = sky_util(pathsky, pathskymap);
    auto image = sky.image_generator(3);
    auto starpairs = starpairs_util(sky, pathstarpairs);
//    NOMAD nomad(starpairs);
//    auto id2 = nomad.run(image["pixels"]);
    SETTLER settler(starpairs);
    auto id1 = settler.run(image["pixels"]);
    return 0;
}

Sky sky_util(string &pathsky_, string &pathskymap_) {
    Sky sky;
    try {
        ifstream is1(pathsky_, ios::binary);
        cereal::BinaryInputArchive iarchive1(is1);
        iarchive1(sky);
    } catch (...) {
        sky.generate(pathskymap_);
        ofstream os1(pathsky_, ios::binary);
        cereal::BinaryOutputArchive oarchive1(os1);
        oarchive1(sky);
    }
    return sky;
}

Starpairs starpairs_util(Sky &sky, string &pathstarpairs_) {
    Starpairs starpairs;
    try {
        ifstream is2(pathstarpairs_, ios::binary);
        cereal::BinaryInputArchive iarchive2(is2);
        iarchive2(starpairs);
    } catch (...) {
        starpairs.generate(sky);
        ofstream os2(pathstarpairs_, ios::binary);
        cereal::BinaryOutputArchive oarchive2(os2);
        oarchive2(starpairs);
    }
    return starpairs;
}
