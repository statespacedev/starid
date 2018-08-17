#include <Eigen/Core>
#include <cereal/archives/binary.hpp>
#include "util.h"
#include "sky.h"

int main() {
    starid::sky sky;
    std::string skydata = "/home/noah/starid/data/sky";
    std::ifstream is1(skydata);
    cereal::BinaryInputArchive iarchive1(is1);
    iarchive1(sky);
    return 0;
}
