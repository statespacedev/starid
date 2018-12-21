#include <Eigen/Core>
#include <cereal/archives/binary.hpp>
#include "util.h"
#include "sky.h"
#include "startriangles.h"
#include <fstream>

int main() {
    starid::sky sky;
    std::string pathsky = "/home/noah/starid/data/sky";
    std::ifstream is1(pathsky);
    cereal::BinaryInputArchive iarchive1(is1);
    iarchive1(sky);
    starid::starpairs starpairs;
    std::string pathstarpairs = "/home/noah/starid/data/pairs";
    std::ifstream is2(pathstarpairs);
    cereal::BinaryInputArchive iarchive2(is2);
    iarchive2(starpairs);

    std::map<std::string, Eigen::MatrixXd> result = sky.image_generator(2);
    Eigen::MatrixXd pixels = result["pixels"];

    starid::startriangles st(starpairs);
    int id = st.identify(pixels);

    return 0;
}
